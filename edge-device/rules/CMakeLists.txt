add_library(rules
        HighTempRule.hpp
        HighTempRule.cpp
        HumiditySpikeRule.hpp
        HumiditySpikeRule.cpp
        ISmartRule.hpp
        MotionAtNightRule.hpp
        MotionAtNightRule.cpp
        SmartRuleEngine.hpp
        SmartRuleEngine.cpp
)
target_include_directories(rules PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/../../mqtt
        ${CMAKE_CURRENT_SOURCE_DIR}/../../utils
)