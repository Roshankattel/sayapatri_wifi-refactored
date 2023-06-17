#include "notify.h"

#include "utils.h"
#include "config.h"

void notifyProcess()
{
    if (httpCode == 200)
    {
        httpCode = 0;
        userName.toUpperCase();
        clearLCD();

        String prompt = rechargeRequest ? "RECHARGE" : "PAYMENT";

        writeString("Dear " + userName + ", your", 160, 130);
        writeString(prompt + " for amount ", 160, 160);
        writeString("Rs." + String(amount) + " is Sucessful!", 160, 190);

        playBuzz(SUCCESS);
    }
    else if (httpCode == 404)
    {
        httpCode = 0;
        showError("User Unknown or", "Insufficient Balance");
    }
    else if (httpCode != 0)
    {
        httpCode = 0;
        showError("Error in Transaction");
    }
}