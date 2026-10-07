local STATE_UNINIT: number = 0
local STATE_INIT: number = 1
local STATE_LOAD: number = 2
local STATE_READY: number = 3
local STATE_ACTIVE: number = 4
local STATE_FINISH: number = 5

local RESULT_NONE: number = 0
local RESULT_SUCCESS: number = 1
local RESULT_FAIL: number = 2
local RESULT_ABORT: number = 3
local RESULT_CANCEL: number = 4

function CheckState(objective, state)
    return objective.state == state
end

function CheckResult(objective, result)
    return objective.result == result
end