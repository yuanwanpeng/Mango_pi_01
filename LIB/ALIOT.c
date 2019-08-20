#include "ALIOT.h"


void utils_hmac_sha1(char *msg, char *key, char *digest)
{
    iot_sha1_context context;
    unsigned char k_ipad[64];    /* inner padding - key XORd with ipad  */
    unsigned char k_opad[64];    /* outer padding - key XORd with opad */
    unsigned char out[20];
    int i;

    /* start out by storing key in pads */
    memset(k_ipad, 0, sizeof(k_ipad));
    memset(k_opad, 0, sizeof(k_opad));
    strcpy((char*)k_ipad, key);
    strcpy((char*)k_opad, key);

    /* XOR key with ipad and opad values */
    for (i = 0; i < 64; i++) {
        k_ipad[i] ^= 0x36;
        k_opad[i] ^= 0x5c;
    }

    /* perform inner SHA */
    utils_sha1_init(&context);                                      /* init context for 1st pass */
    utils_sha1_starts(&context);                                    /* setup context for 1st pass */
    utils_sha1_update(&context, k_ipad, 64);            /* start with inner pad */
    utils_sha1_update(&context, (unsigned char *) msg, strlen(msg));    /* then text of datagram */
    utils_sha1_finish(&context, out);                               /* finish up 1st pass */

    /* perform outer SHA */
    utils_sha1_init(&context);                              /* init context for 2nd pass */
    utils_sha1_starts(&context);                            /* setup context for 2nd pass */
    utils_sha1_update(&context, k_opad, 64);    /* start with outer pad */
    utils_sha1_update(&context, out, 20);     /* then results of 1st hash */
    utils_sha1_finish(&context, out);                       /* finish up 2nd pass */

    for (i = 0; i < 20; ++i) {
        digest[i * 2] = utils_hb2hex(out[i] >> 4);
        digest[i * 2 + 1] = utils_hb2hex(out[i]);
    }
}


void Aliot_Make_Dat(Aliot_TypeDef *	p, uint32_t time)
{
	char a[16] = "\0";
	memset (p, 0 ,sizeof(Aliot_TypeDef));
	
	strcpy(p->IOT_ProductKey,	ProductKey);
	
	strcpy(p->IOT_DeviceName,	p_GPRS->IMEI);
	
	strcpy(p->IOT_DeviceID,		ProductKey);
	strcat(p->IOT_DeviceID,		".");
	strcat(p->IOT_DeviceID,		p_GPRS->IMEI);
	
	//strcpy(p->IOT_DeviceSecret, DeviceSecret);
	strcpy(p->IOT_DeviceSecret, p_GPRS->DS);

	strcpy(p->IOT_Host,				ProductKey);
	strcat(p->IOT_Host,				".");
	strcat(p->IOT_Host,				Host);

	strcpy(p->IOT_Port,				Port);

	strcpy(p->IOT_ClientID,		p->IOT_DeviceID);
	strcat(p->IOT_ClientID,		"|");
	strcat(p->IOT_ClientID,		SecureMode);
	strcat(p->IOT_ClientID,		",");
	strcat(p->IOT_ClientID,		Timestamp);
	strcat(p->IOT_ClientID,		"=");
	strcat(p->IOT_ClientID,		itoa(a,time,10));	
	//strcat(p->IOT_ClientID,		"2524608000000");
	strcat(p->IOT_ClientID,		",");
	strcat(p->IOT_ClientID,		SignMethod);
	strcat(p->IOT_ClientID,		",");
	strcat(p->IOT_ClientID,		Gw);
	strcat(p->IOT_ClientID,		"|");

	strcpy(p->IOT_UserName,		p_GPRS->IMEI);
	strcat(p->IOT_UserName,		"&");
	strcat(p->IOT_UserName,		ProductKey);

	sprintf(p->IOT_Content,	"clientId%s" "deviceName%s" "productKey%s" "timestamp%s",p->IOT_DeviceID, p->IOT_DeviceName,p->IOT_ProductKey,a);

	utils_hmac_sha1(p->IOT_Content, p->IOT_DeviceSecret, p->IOT_Guider_Sign);
	
}