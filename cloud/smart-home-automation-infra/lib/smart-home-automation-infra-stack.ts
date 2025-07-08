import * as cdk from 'aws-cdk-lib';
import { Construct } from 'constructs';
// import * as sqs from 'aws-cdk-lib/aws-sqs';
// import * as dynamodb from 'aws-cdk-lib/aws-dynamodb';
import * as lambda from 'aws-cdk-lib/aws-lambda';
import {aws_iam, aws_iot} from "aws-cdk-lib";
import * as path from 'path';

export interface SmartHomeAutomationInfraStackProps extends cdk.StackProps {
  tableName: string
  env: {
    account?: string,
    region: string
    prod: boolean // dev / prod
  }
}
export class SmartHomeAutomationInfraStack extends cdk.Stack {
  constructor(scope: Construct, id: string, props: SmartHomeAutomationInfraStackProps) {
    super(scope, id, props);

    console.log("Deploying to prod?: ", props?.env.prod);
    console.log('Creating table: ', props?.tableName);
    console.log('Account number: ', props?.env.account);
    console.log('Region: ', props?.env.region);

    const telemetryTable = new cdk.aws_dynamodb.Table(this, `${props?.tableName}`, {
      tableName: props?.tableName,
      partitionKey: { name: 'deviceId', type: cdk.aws_dynamodb.AttributeType.STRING },
      sortKey: { name: 'timestamp', type: cdk.aws_dynamodb.AttributeType.STRING },
      billingMode: cdk.aws_dynamodb.BillingMode.PAY_PER_REQUEST,
      removalPolicy: props.env.prod ? cdk.RemovalPolicy.RETAIN : cdk.RemovalPolicy.DESTROY
    });

    const telemetryLambda = new lambda.Function(this, 'telemetryLambda', {
      runtime: lambda.Runtime.PYTHON_3_12,
      code: lambda.Code.fromAsset(path.join(__dirname, '../../lambda')),
      handler: 'telemetry_lambda.handler',
      environment: {
        TABLE_NAME: telemetryTable.tableName
      }
    });

    telemetryTable.grantWriteData(telemetryLambda);

    const iotHighTemperatureAlertRuleToLambda = new aws_iot.CfnTopicRule(this, 'HighTemperatureAlertRule', {
      topicRulePayload: {
        ruleDisabled: false,
        sql: `SELECT * FROM '/alerts/temperature'`,
        actions: [
          {
            lambda: {
              functionArn: telemetryLambda.functionArn
            }
          }
        ]
      }
    });

    const iotHumiditySpikeAlertRuleToLambda = new aws_iot.CfnTopicRule(this, 'HumiditySpikeAlertRule', {
      topicRulePayload: {
        ruleDisabled: false,
        sql: `SELECT * FROM '/alerts/humidity'`,
        actions: [
          {
            lambda: {
              functionArn: telemetryLambda.functionArn
            }
          }
        ]
      }
    });

    const iotMotionAtNightAlertRuleToLambda = new aws_iot.CfnTopicRule(this, 'MotionAtNightAlertRule', {
      topicRulePayload: {
        description: 'triggered when motion is detected at night => Light is turned ON',
        ruleDisabled: false,
        sql: `SELECT * FROM '/alerts/light'`,
        actions: [
          {
            lambda: {
              functionArn: telemetryLambda.functionArn
            }
          }
        ]
      }
    });

    telemetryLambda.addPermission('AllowIoTTemperature', {
      principal: new aws_iam.ServicePrincipal('iot.amazonaws.com'),
      action: 'lambda:InvokeFunction',
      sourceArn:iotHighTemperatureAlertRuleToLambda.attrArn
    });

    telemetryLambda.addPermission('AllowIoTHumidity', {
      principal: new aws_iam.ServicePrincipal('iot.amazonaws.com'),
      action: 'lambda:InvokeFunction',
      sourceArn: iotHumiditySpikeAlertRuleToLambda.attrArn,
    });

    telemetryLambda.addPermission('AllowIoTMotion-Light', {
      principal: new aws_iam.ServicePrincipal('iot.amazonaws.com'),
      action: 'lambda:InvokeFunction',
      sourceArn: iotMotionAtNightAlertRuleToLambda.attrArn,
    });
  }
}


