#include "welcome.h"
#include "displayFunction.h"
#include "config.h"

static uint32_t sum = 0;

void swtichscan()
{
    rechargeRequest = sum % 10 == 0 ? true : false;
    amount = sum / 10;
    sum = 0;       // reset sum to 0 for next entry
    bleValue = ""; // clear BLE value

    debugln("rechargeRequest:" + String(rechargeRequest));
    debugln("Amount:" + String(amount));
    debugln("THE FREE HEAP IS = " + String(ESP.getFreeHeap()));

    scanPage(rechargeRequest, amount);
}

uint8_t welcomeProcess()
{

    if (bleValue.length() > 1)
    {

        debugln("\n<<< New value >>>");
        for (int i = 0; i < bleValue.length(); i++)
        {
            int num = bleValue[i] - DIGIT_ZERO_ASCII_VALUE;
            if (num >= 0 && num <= 9 && sum <= (AMOUNT_LIMIT / 10))
            {
                sum = sum * 10 + num;
                debugln(sum);
            }
        }

        if (sum > 1)
        {
            swtichscan();
            return 1;
        }
    }

    // Input from the keyboard
    if (Serial.available())
    {
        int num = Serial.read() - DIGIT_ZERO_ASCII_VALUE;
        if (num >= 0 && num <= 9 && sum <= (AMOUNT_LIMIT / 10))
        {
            sum = sum * 10 + num;
            debugln(sum);
        }
        else if (num == -35 && sum > 1)
        {
            swtichscan();
            return 1;
        }
    }

    return 0;
}
