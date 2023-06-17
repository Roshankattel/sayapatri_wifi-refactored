#include "welcome.h"
#include "displayFunction.h"
#include "config.h"

uint8_t welcomeProcess()
{

    if (value.length() > 1)
    {

        debugln("\n<<< New value >>>");

        for (int i = 0; i < value.length(); i++)
        {

            for (int i = 0; i < value.length(); i++)
            {
                int num = value[i] - DIGIT_ZERO_ASCII_VALUE;
                if (num >= 0 && num <= 9 && sum <= (AMOUNT_LIMIT / 10))
                {
                    sum = sum * 10 + num;
                    debugln(sum);
                }
            }
        }
        if (sum > 1)
        {
            rechargeRequest = (sum % 10 == 0);
            amount = sum / 10;
            sum = 0;    // reset sum to 0 for next entry
            value = ""; // clear BLE value

            scanPage(rechargeRequest, amount);

            debugln("rechargeRequest:" + rechargeRequest);
            debugln("Amount:" + String(amount));
            debugln("THE FREE HEAP IS = " + String(ESP.getFreeHeap()));

            return 1;
        }
    }

    // Input from the keyboard
    if (Serial.available())
    {
        int num = Serial.read() - DIGIT_ZERO_ASCII_VALUE;
        static int sum = 0;
        if (num >= 0 && num <= 9 && sum <= (AMOUNT_LIMIT / 10))
        {
            sum = sum * 10 + num;
            debugln(sum);
        }
        else if (num == -35 && sum > 1)
        {
            bool rechargeRequest = (sum % 10 == 0);
            amount = sum / 10;
            sum = 0;
            scanPage(rechargeRequest, amount);
            return 1;
        }
    }

    return 0;
}
