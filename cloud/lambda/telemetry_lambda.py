import os
import boto3
import json
import logging
from datetime import datetime, timezone
from decimal import Decimal



def handler(event, context):
    logger = logging.getLogger()
    logger.setLevel(logging.INFO)

    dynamodb = boto3.resource('dynamodb')
    table = dynamodb.Table(os.environ['TABLE_NAME'])



    logging.info("Processing %d records", len(event.get('Records', [])))

    logging.info( "Received event: %s", json.dumps(event, indent=2))

    logging.info(f"Function name: {context.function_name}, Request ID: {context.aws_request_id}")

    records = event.get('Records', [])

    # IoT direct invoke (single payload, not SQS)
    if not records:
        records = [{'body': json.dumps(event)}]
    try:
        with table.batch_writer() as batch:
            for record in records:
                try:
                    payload = json.loads(record['body']) # payload = convert_floats(json.loads(record['body']))

                    if not payload.get('device_id'):
                        logging.warning("Skipping record without device_id")
                        continue

                    telemetry_data = {
                        'device_id': payload['device_id'],
                        'timestamp': payload.get('timestamp', datetime.now(timezone.utc).isoformat())
                    }

                    for field in ['rule', 'sensor', 'value', 'message']:
                        if payload.get(field) is not None:
                            val =  payload[field]
                            if isinstance(val, float):
                                val = Decimal(str(val))
                            telemetry_data[field] = val

                    batch.put_item(Item=telemetry_data)
                    logging.info("Queued telemetry for device: %s", payload['device_id'])

                except json.JSONDecodeError as e:
                    logging.error("Invalid JSON in record: %s", str(e))
                    continue
                except Exception as e:
                    logging.error("Error processing record: %s", str(e))
                    continue

        return {
            'statusCode': 200,
            'body': json.dumps("Successfully stored telemetry")
        }

    except Exception as e:
        logging.error("Critical error in handler: %s", str(e))
        return {
            'statusCode': 500,
            'body': json.dumps("Error processing telemetry data")
        }
