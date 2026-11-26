// FlightRadar v7.3.6 - material-inspired UI, staged details, bounded vector map. ESP32 4MB: Huge APP (3MB No OTA/1MB SPIFFS).
#include <TFT_eSPI.h>
#include <SPI.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Preferences.h>
#include <FS.h>
using fs::FS;
#include <WebServer.h>
#include <TJpg_Decoder.h>
#include <PNGdec.h>
#include <memory>
#include <new>
#include <math.h>
#include <ctype.h>
#include <time.h>
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>
#include <freertos/task.h>
static const char ROOTS[] PROGMEM = R"PEM(-----BEGIN CERTIFICATE-----
MIIFazCCA1OgAwIBAgIRAIIQz7DSQONZRGPgu2OCiwAwDQYJKoZIhvcNAQELBQAw
TzELMAkGA1UEBhMCVVMxKTAnBgNVBAoTIEludGVybmV0IFNlY3VyaXR5IFJlc2Vh
cmNoIEdyb3VwMRUwEwYDVQQDEwxJU1JHIFJvb3QgWDEwHhcNMTUwNjA0MTEwNDM4
WhcNMzUwNjA0MTEwNDM4WjBPMQswCQYDVQQGEwJVUzEpMCcGA1UEChMgSW50ZXJu
ZXQgU2VjdXJpdHkgUmVzZWFyY2ggR3JvdXAxFTATBgNVBAMTDElTUkcgUm9vdCBY
MTCCAiIwDQYJKoZIhvcNAQEBBQADggIPADCCAgoCggIBAK3oJHP0FDfzm54rVygc
h77ct984kIxuPOZXoHj3dcKi/vVqbvYATyjb3miGbESTtrFj/RQSa78f0uoxmyF+
0TM8ukj13Xnfs7j/EvEhmkvBioZxaUpmZmyPfjxwv60pIgbz5MDmgK7iS4+3mX6U
A5/TR5d8mUgjU+g4rk8Kb4Mu0UlXjIB0ttov0DiNewNwIRt18jA8+o+u3dpjq+sW
T8KOEUt+zwvo/7V3LvSye0rgTBIlDHCNAymg4VMk7BPZ7hm/ELNKjD+Jo2FR3qyH
B5T0Y3HsLuJvW5iB4YlcNHlsdu87kGJ55tukmi8mxdAQ4Q7e2RCOFvu396j3x+UC
B5iPNgiV5+I3lg02dZ77DnKxHZu8A/lJBdiB3QW0KtZB6awBdpUKD9jf1b0SHzUv
KBds0pjBqAlkd25HN7rOrFleaJ1/ctaJxQZBKT5ZPt0m9STJEadao0xAH0ahmbWn
OlFuhjuefXKnEgV4We0+UXgVCwOPjdAvBbI+e0ocS3MFEvzG6uBQE3xDk3SzynTn
jh8BCNAw1FtxNrQHusEwMFxIt4I7mKZ9YIqioymCzLq9gwQbooMDQaHWBfEbwrbw
qHyGO0aoSCqI3Haadr8faqU9GY/rOPNk3sgrDQoo//fb4hVC1CLQJ13hef4Y53CI
rU7m2Ys6xt0nUW7/vGT1M0NPAgMBAAGjQjBAMA4GA1UdDwEB/wQEAwIBBjAPBgNV
HRMBAf8EBTADAQH/MB0GA1UdDgQWBBR5tFnme7bl5AFzgAiIyBpY9umbbjANBgkq
hkiG9w0BAQsFAAOCAgEAVR9YqbyyqFDQDLHYGmkgJykIrGF1XIpu+ILlaS/V9lZL
ubhzEFnTIZd+50xx+7LSYK05qAvqFyFWhfFQDlnrzuBZ6brJFe+GnY+EgPbk6ZGQ
3BebYhtF8GaV0nxvwuo77x/Py9auJ/GpsMiu/X1+mvoiBOv/2X/qkSsisRcOj/KK
NFtY2PwByVS5uCbMiogziUwthDyC3+6WVwW6LLv3xLfHTjuCvjHIInNzktHCgKQ5
ORAzI4JMPJ+GslWYHb4phowim57iaztXOoJwTdwJx4nLCgdNbOhdjsnvzqvHu7Ur
TkXWStAmzOVyyghqpZXjFaH3pO3JLF+l+/+sKAIuvtd7u+Nxe5AW0wdeRlN8NwdC
jNPElpzVmbUq4JUagEiuTDkHzsxHpFKVK7q4+63SM1N95R1NbdWhscdCb+ZAJzVc
oyi3B43njTOQ5yOf+1CceWxG1bQVs5ZufpsMljq4Ui0/1lvh+wjChP4kqKOJ2qxq
4RgqsahDYVvTH9w7jXbyLeiNdd8XM2w9U/t7y0Ff/9yi0GE44Za4rF2LN9d11TPA
mRGunUHBcnWEvgJBQl9nJEiU0Zsnvgc/ubhPgXRR4Xq37Z0j4r7g1SgEEzwxA57d
emyPxgcYxn/eR44/KJ4EBs+lVDR3veyJm+kXQ99b21/+jh5Xos1AnX5iItreGCc=
-----END CERTIFICATE-----
-----BEGIN CERTIFICATE-----
MIICGzCCAaGgAwIBAgIQQdKd0XLq7qeAwSxs6S+HUjAKBggqhkjOPQQDAzBPMQsw
CQYDVQQGEwJVUzEpMCcGA1UEChMgSW50ZXJuZXQgU2VjdXJpdHkgUmVzZWFyY2gg
R3JvdXAxFTATBgNVBAMTDElTUkcgUm9vdCBYMjAeFw0yMDA5MDQwMDAwMDBaFw00
MDA5MTcxNjAwMDBaME8xCzAJBgNVBAYTAlVTMSkwJwYDVQQKEyBJbnRlcm5ldCBT
ZWN1cml0eSBSZXNlYXJjaCBHcm91cDEVMBMGA1UEAxMMSVNSRyBSb290IFgyMHYw
EAYHKoZIzj0CAQYFK4EEACIDYgAEzZvVn4CDCuwJSvMWSj5cz3es3mcFDR0HttwW
+1qLFNvicWDEukWVEYmO6gbf9yoWHKS5xcUy4APgHoIYOIvXRdgKam7mAHf7AlF9
ItgKbppbd9/w+kHsOdx1ymgHDB/qo0IwQDAOBgNVHQ8BAf8EBAMCAQYwDwYDVR0T
AQH/BAUwAwEB/zAdBgNVHQ4EFgQUfEKWrt5LSDv6kviejM9ti6lyN5UwCgYIKoZI
zj0EAwMDaAAwZQIwe3lORlCEwkSHRhtFcP9Ymd70/aTSVaYgLXTWNLxBo1BfASdW
tL4ndQavEi51mI38AjEAi/V3bNTIZargCyzuFJ0nN6T5U6VR5CmD1/iQMVtCnwr1
/q4AaOeMSQ+2b1tbFfLn
-----END CERTIFICATE-----
-----BEGIN CERTIFICATE-----
MIIFVzCCAz+gAwIBAgINAgPlk28xsBNJiGuiFzANBgkqhkiG9w0BAQwFADBHMQsw
CQYDVQQGEwJVUzEiMCAGA1UEChMZR29vZ2xlIFRydXN0IFNlcnZpY2VzIExMQzEU
MBIGA1UEAxMLR1RTIFJvb3QgUjEwHhcNMTYwNjIyMDAwMDAwWhcNMzYwNjIyMDAw
MDAwWjBHMQswCQYDVQQGEwJVUzEiMCAGA1UEChMZR29vZ2xlIFRydXN0IFNlcnZp
Y2VzIExMQzEUMBIGA1UEAxMLR1RTIFJvb3QgUjEwggIiMA0GCSqGSIb3DQEBAQUA
A4ICDwAwggIKAoICAQC2EQKLHuOhd5s73L+UPreVp0A8of2C+X0yBoJx9vaMf/vo
27xqLpeXo4xL+Sv2sfnOhB2x+cWX3u+58qPpvBKJXqeqUqv4IyfLpLGcY9vXmX7w
Cl7raKb0xlpHDU0QM+NOsROjyBhsS+z8CZDfnWQpJSMHobTSPS5g4M/SCYe7zUjw
TcLCeoiKu7rPWRnWr4+wB7CeMfGCwcDfLqZtbBkOtdh+JhpFAz2weaSUKK0Pfybl
qAj+lug8aJRT7oM6iCsVlgmy4HqMLnXWnOunVmSPlk9orj2XwoSPwLxAwAtcvfaH
szVsrBhQf4TgTM2S0yDpM7xSma8ytSmzJSq0SPly4cpk9+aCEI3oncKKiPo4Zor8
Y/kB+Xj9e1x3+naH+uzfsQ55lVe0vSbv1gHR6xYKu44LtcXFilWr06zqkUspzBmk
MiVOKvFlRNACzqrOSbTqn3yDsEB750Orp2yjj32JgfpMpf/VjsPOS+C12LOORc92
wO1AK/1TD7Cn1TsNsYqiA94xrcx36m97PtbfkSIS5r762DL8EGMUUXLeXdYWk70p
aDPvOmbsB4om3xPXV2V4J95eSRQAogB/mqghtqmxlbCluQ0WEdrHbEg8QOB+DVrN
VjzRlwW5y0vtOUucxD/SVRNuJLDWcfr0wbrM7Rv1/oFB2ACYPTrIrnqYNxgFlQID
AQABo0IwQDAOBgNVHQ8BAf8EBAMCAYYwDwYDVR0TAQH/BAUwAwEB/zAdBgNVHQ4E
FgQU5K8rJnEaK0gnhS9SZizv8IkTcT4wDQYJKoZIhvcNAQEMBQADggIBAJ+qQibb
C5u+/x6Wki4+omVKapi6Ist9wTrYggoGxval3sBOh2Z5ofmmWJyq+bXmYOfg6LEe
QkEzCzc9zolwFcq1JKjPa7XSQCGYzyI0zzvFIoTgxQ6KfF2I5DUkzps+GlQebtuy
h6f88/qBVRRiClmpIgUxPoLW7ttXNLwzldMXG+gnoot7TiYaelpkttGsN/H9oPM4
7HLwEXWdyzRSjeZ2axfG34arJ45JK3VmgRAhpuo+9K4l/3wV3s6MJT/KYnAK9y8J
ZgfIPxz88NtFMN9iiMG1D53Dn0reWVlHxYciNuaCp+0KueIHoI17eko8cdLiA6Ef
MgfdG+RCzgwARWGAtQsgWSl4vflVy2PFPEz0tv/bal8xa5meLMFrUKTX5hgUvYU/
Z6tGn6D/Qqc6f1zLXbBwHSs09dR2CQzreExZBfMzQsNhFRAbd03OIozUhfJFfbdT
6u9AWpQKXCBfTkBdYiJ23//OYb2MI3jSNwLgjt7RETeJ9r/tSQdirpLsQBqvFAnZ
0E6yove+7u7Y/9waLd64NnHi/Hm3lCXRSHNboTXns5lndcEZOitHTtNCjv0xyBZm
2tIMPNuzjsmhDYAPexZ3FL//2wmUspO8IFgV6dtxQ/PeEMMA3KgqlbbC1j+Qa3bb
bP6MvPJwNQzcmRk13NfIRmPVNnGuV/u3gm3c
-----END CERTIFICATE-----
-----BEGIN CERTIFICATE-----
MIIFVzCCAz+gAwIBAgINAgPlrsWNBCUaqxElqjANBgkqhkiG9w0BAQwFADBHMQsw
CQYDVQQGEwJVUzEiMCAGA1UEChMZR29vZ2xlIFRydXN0IFNlcnZpY2VzIExMQzEU
MBIGA1UEAxMLR1RTIFJvb3QgUjIwHhcNMTYwNjIyMDAwMDAwWhcNMzYwNjIyMDAw
MDAwWjBHMQswCQYDVQQGEwJVUzEiMCAGA1UEChMZR29vZ2xlIFRydXN0IFNlcnZp
Y2VzIExMQzEUMBIGA1UEAxMLR1RTIFJvb3QgUjIwggIiMA0GCSqGSIb3DQEBAQUA
A4ICDwAwggIKAoICAQDO3v2m++zsFDQ8BwZabFn3GTXd98GdVarTzTukk3LvCvpt
nfbwhYBboUhSnznFt+4orO/LdmgUud+tAWyZH8QiHZ/+cnfgLFuv5AS/T3KgGjSY
6Dlo7JUle3ah5mm5hRm9iYz+re026nO8/4Piy33B0s5Ks40FnotJk9/BW9BuXvAu
MC6C/Pq8tBcKSOWIm8Wba96wyrQD8Nr0kLhlZPdcTK3ofmZemde4wj7I0BOdre7k
RXuJVfeKH2JShBKzwkCX44ofR5GmdFrS+LFjKBC4swm4VndAoiaYecb+3yXuPuWg
f9RhD1FLPD+M2uFwdNjCaKH5wQzpoeJ/u1U8dgbuak7MkogwTZq9TwtImoS1mKPV
+3PBV2HdKFZ1E66HjucMUQkQdYhMvI35ezzUIkgfKtzra7tEscszcTJGr61K8Yzo
dDqs5xoic4DSMPclQsciOzsSrZYuxsN2B6ogtzVJV+mSSeh2FnIxZyuWfoqjx5RW
Ir9qS34BIbIjMt/kmkRtWVtd9QCgHJvGeJeNkP+byKq0rxFROV7Z+2et1VsRnTKa
G73VululycslaVNVJ1zgyjbLiGH7HrfQy+4W+9OmTN6SpdTi3/UGVN4unUu0kzCq
gc7dGtxRcw1PcOnlthYhGXmy5okLdWTK1au8CcEYof/UVKGFPP0UJAOyh9OktwID
AQABo0IwQDAOBgNVHQ8BAf8EBAMCAYYwDwYDVR0TAQH/BAUwAwEB/zAdBgNVHQ4E
FgQUu//KjiOfT5nK2+JopqUVJxce2Q4wDQYJKoZIhvcNAQEMBQADggIBAB/Kzt3H
vqGf2SdMC9wXmBFqiN495nFWcrKeGk6c1SuYJF2ba3uwM4IJvd8lRuqYnrYb/oM8
0mJhwQTtzuDFycgTE1XnqGOtjHsB/ncw4c5omwX4Eu55MaBBRTUoCnGkJE+M3DyC
B19m3H0Q/gxhswWV7uGugQ+o+MePTagjAiZrHYNSVc61LwDKgEDg4XSsYPWHgJ2u
NmSRXbBoGOqKYcl3qJfEycel/FVL8/B/uWU9J2jQzGv6U53hkRrJXRqWbTKH7QMg
yALOWr7Z6v2yTcQvG99fevX4i8buMTolUVVnjWQye+mew4K6Ki3pHrTgSAai/Gev
HyICc/sgCq+dVEuhzf9gR7A/Xe8bVr2XIZYtCtFenTgCR2y59PYjJbigapordwj6
xLEokCZYCDzifqrXPW+6MYgKBesntaFJ7qBFVHvmJ2WZICGoo7z7GJa7Um8M7YNR
TOlZ4iBgxcJlkoKM8xAfDoqXvneCbT+PHV28SSe9zE8P4c52hgQjxcCMElv924Sg
JPFI/2R80L5cFtHvma3AH/vLrrw4IgYmZNralw4/KBVEqE8AyvCazM90arQ+POuV
7LXTWtiBmelDGDfrs7vRWGJB82bSj6p4lVQgw1oudCvV0b4YacCs1aTPObpRhANl
6WLAYv7YTVWW4tAR+kg0Eeye7QUd5MjWHYbL
-----END CERTIFICATE-----
-----BEGIN CERTIFICATE-----
MIICCTCCAY6gAwIBAgINAgPluILrIPglJ209ZjAKBggqhkjOPQQDAzBHMQswCQYD
VQQGEwJVUzEiMCAGA1UEChMZR29vZ2xlIFRydXN0IFNlcnZpY2VzIExMQzEUMBIG
A1UEAxMLR1RTIFJvb3QgUjMwHhcNMTYwNjIyMDAwMDAwWhcNMzYwNjIyMDAwMDAw
WjBHMQswCQYDVQQGEwJVUzEiMCAGA1UEChMZR29vZ2xlIFRydXN0IFNlcnZpY2Vz
IExMQzEUMBIGA1UEAxMLR1RTIFJvb3QgUjMwdjAQBgcqhkjOPQIBBgUrgQQAIgNi
AAQfTzOHMymKoYTey8chWEGJ6ladK0uFxh1MJ7x/JlFyb+Kf1qPKzEUURout736G
jOyxfi//qXGdGIRFBEFVbivqJn+7kAHjSxm65FSWRQmx1WyRRK2EE46ajA2ADDL2
4CejQjBAMA4GA1UdDwEB/wQEAwIBhjAPBgNVHRMBAf8EBTADAQH/MB0GA1UdDgQW
BBTB8Sa6oC2uhYHP0/EqEr24Cmf9vDAKBggqhkjOPQQDAwNpADBmAjEA9uEglRR7
VKOQFhG/hMjqb2sXnh5GmCCbn9MN2azTL818+FsuVbu/3ZL3pAzcMeGiAjEA/Jdm
ZuVDFhOD3cffL74UOO0BzrEXGhF16b0DjyZ+hOXJYKaV11RZt+cRLInUue4X
-----END CERTIFICATE-----
-----BEGIN CERTIFICATE-----
MIICCTCCAY6gAwIBAgINAgPlwGjvYxqccpBQUjAKBggqhkjOPQQDAzBHMQswCQYD
VQQGEwJVUzEiMCAGA1UEChMZR29vZ2xlIFRydXN0IFNlcnZpY2VzIExMQzEUMBIG
A1UEAxMLR1RTIFJvb3QgUjQwHhcNMTYwNjIyMDAwMDAwWhcNMzYwNjIyMDAwMDAw
WjBHMQswCQYDVQQGEwJVUzEiMCAGA1UEChMZR29vZ2xlIFRydXN0IFNlcnZpY2Vz
IExMQzEUMBIGA1UEAxMLR1RTIFJvb3QgUjQwdjAQBgcqhkjOPQIBBgUrgQQAIgNi
AATzdHOnaItgrkO4NcWBMHtLSZ37wWHO5t5GvWvVYRg1rkDdc/eJkTBa6zzuhXyi
QHY7qca4R9gq55KRanPpsXI5nymfopjTX15YhmUPoYRlBtHci8nHc8iMai/lxKvR
HYqjQjBAMA4GA1UdDwEB/wQEAwIBhjAPBgNVHRMBAf8EBTADAQH/MB0GA1UdDgQW
BBSATNbrdP9JNqPV2Py1PsVq8JQdjDAKBggqhkjOPQQDAwNpADBmAjEA6ED/g94D
9J+uHXqnLrmvT/aDHQ4thQEd0dlq7A/Cr8deVl5c1RxYIigL9zC2L7F8AjEA8GE8
p/SgguMh1YQdc4acLa/KNJvxn7kjNuK8YAOdgLOaVsjh4rsUecrNIdSUtUlD
-----END CERTIFICATE-----
-----BEGIN CERTIFICATE-----
MIIDrzCCApegAwIBAgIQCDvgVpBCRrGhdWrJWZHHSjANBgkqhkiG9w0BAQUFADBh
MQswCQYDVQQGEwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMRkwFwYDVQQLExB3
d3cuZGlnaWNlcnQuY29tMSAwHgYDVQQDExdEaWdpQ2VydCBHbG9iYWwgUm9vdCBD
QTAeFw0wNjExMTAwMDAwMDBaFw0zMTExMTAwMDAwMDBaMGExCzAJBgNVBAYTAlVT
MRUwEwYDVQQKEwxEaWdpQ2VydCBJbmMxGTAXBgNVBAsTEHd3dy5kaWdpY2VydC5j
b20xIDAeBgNVBAMTF0RpZ2lDZXJ0IEdsb2JhbCBSb290IENBMIIBIjANBgkqhkiG
9w0BAQEFAAOCAQ8AMIIBCgKCAQEA4jvhEXLeqKTTo1eqUKKPC3eQyaKl7hLOllsB
CSDMAZOnTjC3U/dDxGkAV53ijSLdhwZAAIEJzs4bg7/fzTtxRuLWZscFs3YnFo97
nh6Vfe63SKMI2tavegw5BmV/Sl0fvBf4q77uKNd0f3p4mVmFaG5cIzJLv07A6Fpt
43C/dxC//AH2hdmoRBBYMql1GNXRor5H4idq9Joz+EkIYIvUX7Q6hL+hqkpMfT7P
T19sdl6gSzeRntwi5m3OFBqOasv+zbMUZBfHWymeMr/y7vrTC0LUq7dBMtoM1O/4
gdW7jVg/tRvoSSiicNoxBN33shbyTApOB6jtSj1etX+jkMOvJwIDAQABo2MwYTAO
BgNVHQ8BAf8EBAMCAYYwDwYDVR0TAQH/BAUwAwEB/zAdBgNVHQ4EFgQUA95QNVbR
TLtm8KPiGxvDl7I90VUwHwYDVR0jBBgwFoAUA95QNVbRTLtm8KPiGxvDl7I90VUw
DQYJKoZIhvcNAQEFBQADggEBAMucN6pIExIK+t1EnE9SsPTfrgT1eXkIoyQY/Esr
hMAtudXH/vTBH1jLuG2cenTnmCmrEbXjcKChzUyImZOMkXDiqw8cvpOp/2PV5Adg
06O/nVsJ8dWO41P0jmP6P6fbtGbfYmbW0W5BjfIttep3Sp+dWOIrWcBAI+0tKIJF
PnlUkiaY4IBIqDfv8NZ5YBberOgOzW6sRBc4L0na4UU+Krk2U886UAb3LujEV0ls
YSEY1QSteDwsOoBrp+uvFRTp2InBuThs4pFsiv9kuXclVzDAGySj4dzp30d8tbQk
CAUw7C29C79Fv1C5qfPrmAESrciIxpg0X40KPMbp1ZWVbd4=
-----END CERTIFICATE-----
-----BEGIN CERTIFICATE-----
MIIDjjCCAnagAwIBAgIQAzrx5qcRqaC7KGSxHQn65TANBgkqhkiG9w0BAQsFADBh
MQswCQYDVQQGEwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMRkwFwYDVQQLExB3
d3cuZGlnaWNlcnQuY29tMSAwHgYDVQQDExdEaWdpQ2VydCBHbG9iYWwgUm9vdCBH
MjAeFw0xMzA4MDExMjAwMDBaFw0zODAxMTUxMjAwMDBaMGExCzAJBgNVBAYTAlVT
MRUwEwYDVQQKEwxEaWdpQ2VydCBJbmMxGTAXBgNVBAsTEHd3dy5kaWdpY2VydC5j
b20xIDAeBgNVBAMTF0RpZ2lDZXJ0IEdsb2JhbCBSb290IEcyMIIBIjANBgkqhkiG
9w0BAQEFAAOCAQ8AMIIBCgKCAQEAuzfNNNx7a8myaJCtSnX/RrohCgiN9RlUyfuI
2/Ou8jqJkTx65qsGGmvPrC3oXgkkRLpimn7Wo6h+4FR1IAWsULecYxpsMNzaHxmx
1x7e/dfgy5SDN67sH0NO3Xss0r0upS/kqbitOtSZpLYl6ZtrAGCSYP9PIUkY92eQ
q2EGnI/yuum06ZIya7XzV+hdG82MHauVBJVJ8zUtluNJbd134/tJS7SsVQepj5Wz
tCO7TG1F8PapspUwtP1MVYwnSlcUfIKdzXOS0xZKBgyMUNGPHgm+F6HmIcr9g+UQ
vIOlCsRnKPZzFBQ9RnbDhxSJITRNrw9FDKZJobq7nMWxM4MphQIDAQABo0IwQDAP
BgNVHRMBAf8EBTADAQH/MA4GA1UdDwEB/wQEAwIBhjAdBgNVHQ4EFgQUTiJUIBiV
5uNu5g/6+rkS7QYXjzkwDQYJKoZIhvcNAQELBQADggEBAGBnKJRvDkhj6zHd6mcY
1Yl9PMWLSn/pvtsrF9+wX3N3KjITOYFnQoQj8kVnNeyIv/iPsGEMNKSuIEyExtv4
NeF22d+mQrvHRAiGfzZ0JFrabA0UWTW98kndth/Jsw1HKj2ZL7tcu7XUIOGZX1NG
Fdtom/DzMNU+MeKNhJ7jitralj41E6Vf8PlwUHBHQRFXGU7Aj64GxJUTFy8bJZ91
8rGOmaFvE7FBcf6IKshPECBV1/MUReXgRPTqh5Uykw7+U0b6LJ3/iyK5S9kJRaTe
pLiaWN0bfVKfjllDiIGknibVb63dDcY3fe0Dkhvld1927jyNxF1WW6LZZm6zNTfl
MrY=
-----END CERTIFICATE-----
-----BEGIN CERTIFICATE-----
MIICPzCCAcWgAwIBAgIQBVVWvPJepDU1w6QP1atFcjAKBggqhkjOPQQDAzBhMQsw
CQYDVQQGEwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMRkwFwYDVQQLExB3d3cu
ZGlnaWNlcnQuY29tMSAwHgYDVQQDExdEaWdpQ2VydCBHbG9iYWwgUm9vdCBHMzAe
Fw0xMzA4MDExMjAwMDBaFw0zODAxMTUxMjAwMDBaMGExCzAJBgNVBAYTAlVTMRUw
EwYDVQQKEwxEaWdpQ2VydCBJbmMxGTAXBgNVBAsTEHd3dy5kaWdpY2VydC5jb20x
IDAeBgNVBAMTF0RpZ2lDZXJ0IEdsb2JhbCBSb290IEczMHYwEAYHKoZIzj0CAQYF
K4EEACIDYgAE3afZu4q4C/sLfyHS8L6+c/MzXRq8NOrexpu80JX28MzQC7phW1FG
fp4tn+6OYwwX7Adw9c+ELkCDnOg/QW07rdOkFFk2eJ0DQ+4QE2xy3q6Ip6FrtUPO
Z9wj/wMco+I+o0IwQDAPBgNVHRMBAf8EBTADAQH/MA4GA1UdDwEB/wQEAwIBhjAd
BgNVHQ4EFgQUs9tIpPmhxdiuNkHMEWNpYim8S8YwCgYIKoZIzj0EAwMDaAAwZQIx
AK288mw/EkrRLTnDCgmXc/SINoyIJ7vmiI1Qhadj+Z4y3maTD/HMsQmP3Wyr+mt/
oAIwOWZbwmSNuJ5Q3KjVSaLtx9zRSX8XAbjIho9OjIgrqJqpisXRAL34VOKa5Vt8
sycX
-----END CERTIFICATE-----
-----BEGIN CERTIFICATE-----
MIIFZjCCA06gAwIBAgIQCPm0eKj6ftpqMzeJ3nzPijANBgkqhkiG9w0BAQwFADBN
MQswCQYDVQQGEwJVUzEXMBUGA1UEChMORGlnaUNlcnQsIEluYy4xJTAjBgNVBAMT
HERpZ2lDZXJ0IFRMUyBSU0E0MDk2IFJvb3QgRzUwHhcNMjEwMTE1MDAwMDAwWhcN
NDYwMTE0MjM1OTU5WjBNMQswCQYDVQQGEwJVUzEXMBUGA1UEChMORGlnaUNlcnQs
IEluYy4xJTAjBgNVBAMTHERpZ2lDZXJ0IFRMUyBSU0E0MDk2IFJvb3QgRzUwggIi
MA0GCSqGSIb3DQEBAQUAA4ICDwAwggIKAoICAQCz0PTJeRGd/fxmgefM1eS87IE+
ajWOLrfn3q/5B03PMJ3qCQuZvWxX2hhKuHisOjmopkisLnLlvevxGs3npAOpPxG0
2C+JFvuUAT27L/gTBaF4HI4o4EXgg/RZG5Wzrn4DReW+wkL+7vI8toUTmDKdFqgp
wgscONyfMXdcvyej/Cestyu9dJsXLfKB2l2w4SMXPohKEiPQ6s+d3gMXsUJKoBZM
pG2T6T867jp8nVid9E6P/DsjyG244gXazOvswzH016cpVIDPRFtMbzCe88zdH5RD
nU1/cHAN1DrRN/BsnZvAFJNY781BOHW8EwOVfH/jXOnVDdXifBBiqmvwPXbzP6Po
sMH976pXTayGpxi0KcEsDr9kvimM2AItzVwv8n/vFfQMFawKsPHTDU9qTXeXAaDx
Zre3zu/O7Oyldcqs4+Fj97ihBMi8ez9dLRYiVu1ISf6nL3kwJZu6ay0/nTvEF+cd
Lvvyz6b84xQslpghjLSR6Rlgg/IwKwZzUNWYOwbpx4oMYIwo+FKbbuH2TbsGJJvX
KyY//SovcfXWJL5/MZ4PbeiPT02jP/816t9JXkGPhvnxd3lLG7SjXi/7RgLQZhNe
XoVPzthwiHvOAbWWl9fNff2C+MIkwcoBOU+NosEUQB+cZtUMCUbW8tDRSHZWOkPL
tgoRObqME2wGtZ7P6wIDAQABo0IwQDAdBgNVHQ4EFgQUUTMc7TZArxfTJc1paPKv
TiM+s0EwDgYDVR0PAQH/BAQDAgGGMA8GA1UdEwEB/wQFMAMBAf8wDQYJKoZIhvcN
AQEMBQADggIBAGCmr1tfV9qJ20tQqcQjNSH/0GEwhJG3PxDPJY7Jv0Y02cEhJhxw
GXIeo8mH/qlDZJY6yFMECrZBu8RHANmfGBg7sg7zNOok992vIGCukihfNudd5N7H
PNtQOa27PShNlnx2xlv0wdsUpasZYgcYQF+Xkdycx6u1UQ3maVNVzDl92sURVXLF
O4uJ+DQtpBflF+aZfTCIITfNMBc9uPK8qHWgQ9w+iUuQrm0D4ByjoJYJu32jtyoQ
REtGBzRj7TG5BO6jm5qu5jF49OokYTurWGT/u4cnYiWB39yhL/btp/96j1EuMPik
AdKFOV8BmZZvWltwGUb+hmA+rYAQCd05JS9Yf7vSdPD3Rh9GOUrYU9DzLjtxpdRv
/PNn5AeP3SYZ4Y1b+qOTEZvpyDrDVWiakuFSdjjo4bq9+0/V77PnSIMx8IIh47a+
p6tv75/fTM8BuGJqIz3nCU2AG3swpMPdB380vqQmsvZB6Akd4yCYqjdP//fx4ilw
MUc/dNAUFvohigLVigmUdy7yWSiLfFCSCmZ4OIN1xLVaqBHG5cGdZlXPU8Sv13WF
qUITVuwhd4GTWgzqltlJyqEI8pc7bZsEGCREjnwB8twl2F6GmrE52/WRMmrRpnCK
ovfepEWFJqgejF0pW8hL2JpqA15w8oVPbEtoL8pU9ozaMv7Da4M/OMZ+
-----END CERTIFICATE-----
-----BEGIN CERTIFICATE-----
MIICGTCCAZ+gAwIBAgIQCeCTZaz32ci5PhwLBCou8zAKBggqhkjOPQQDAzBOMQsw
CQYDVQQGEwJVUzEXMBUGA1UEChMORGlnaUNlcnQsIEluYy4xJjAkBgNVBAMTHURp
Z2lDZXJ0IFRMUyBFQ0MgUDM4NCBSb290IEc1MB4XDTIxMDExNTAwMDAwMFoXDTQ2
MDExNDIzNTk1OVowTjELMAkGA1UEBhMCVVMxFzAVBgNVBAoTDkRpZ2lDZXJ0LCBJ
bmMuMSYwJAYDVQQDEx1EaWdpQ2VydCBUTFMgRUNDIFAzODQgUm9vdCBHNTB2MBAG
ByqGSM49AgEGBSuBBAAiA2IABMFEoc8Rl1Ca3iOCNQfN0MsYndLxf3c1TzvdlHJS
7cI7+Oz6e2tYIOyZrsn8aLN1udsJ7MgT9U7GCh1mMEy7H0cKPGEQQil8pQgO4CLp
0zVozptjn4S1mU1YoI71VOeVyaNCMEAwHQYDVR0OBBYEFMFRRVBZqz7nLFr6ICIS
B4CIfBFqMA4GA1UdDwEB/wQEAwIBhjAPBgNVHRMBAf8EBTADAQH/MAoGCCqGSM49
BAMDA2gAMGUCMQCJao1H5+z8blUD2WdsJk6Dxv3J+ysTvLd6jLRl0mlpYxNjOyZQ
LgGheQaRnUi/wr4CMEfDFXuxoJGZSZOoPHzoRgaLLPIxAJSdYsiJvRmEFOml+wG4
DXZDjC5Ty3zfDBeWUA==
-----END CERTIFICATE-----
-----BEGIN CERTIFICATE-----
MIIF3jCCA8agAwIBAgIQAf1tMPyjylGoG7xkDjUDLTANBgkqhkiG9w0BAQwFADCB
iDELMAkGA1UEBhMCVVMxEzARBgNVBAgTCk5ldyBKZXJzZXkxFDASBgNVBAcTC0pl
cnNleSBDaXR5MR4wHAYDVQQKExVUaGUgVVNFUlRSVVNUIE5ldHdvcmsxLjAsBgNV
BAMTJVVTRVJUcnVzdCBSU0EgQ2VydGlmaWNhdGlvbiBBdXRob3JpdHkwHhcNMTAw
MjAxMDAwMDAwWhcNMzgwMTE4MjM1OTU5WjCBiDELMAkGA1UEBhMCVVMxEzARBgNV
BAgTCk5ldyBKZXJzZXkxFDASBgNVBAcTC0plcnNleSBDaXR5MR4wHAYDVQQKExVU
aGUgVVNFUlRSVVNUIE5ldHdvcmsxLjAsBgNVBAMTJVVTRVJUcnVzdCBSU0EgQ2Vy
dGlmaWNhdGlvbiBBdXRob3JpdHkwggIiMA0GCSqGSIb3DQEBAQUAA4ICDwAwggIK
AoICAQCAEmUXNg7D2wiz0KxXDXbtzSfTTK1Qg2HiqiBNCS1kCdzOiZ/MPans9s/B
3PHTsdZ7NygRK0faOca8Ohm0X6a9fZ2jY0K2dvKpOyuR+OJv0OwWIJAJPuLodMkY
tJHUYmTbf6MG8YgYapAiPLz+E/CHFHv25B+O1ORRxhFnRghRy4YUVD+8M/5+bJz/
Fp0YvVGONaanZshyZ9shZrHUm3gDwFA66Mzw3LyeTP6vBZY1H1dat//O+T23LLb2
VN3I5xI6Ta5MirdcmrS3ID3KfyI0rn47aGYBROcBTkZTmzNg95S+UzeQc0PzMsNT
79uq/nROacdrjGCT3sTHDN/hMq7MkztReJVni+49Vv4M0GkPGw/zJSZrM233bkf6
c0Plfg6lZrEpfDKEY1WJxA3Bk1QwGROs0303p+tdOmw1XNtB1xLaqUkL39iAigmT
Yo61Zs8liM2EuLE/pDkP2QKe6xJMlXzzawWpXhaDzLhn4ugTncxbgtNMs+1b/97l
c6wjOy0AvzVVdAlJ2ElYGn+SNuZRkg7zJn0cTRe8yexDJtC/QV9AqURE9JnnV4ee
UB9XVKg+/XRjL7FQZQnmWEIuQxpMtPAlR1n6BB6T1CZGSlCBst6+eLf8ZxXhyVeE
Hg9j1uliutZfVS7qXMYoCAQlObgOK6nyTJccBz8NUvXt7y+CDwIDAQABo0IwQDAd
BgNVHQ4EFgQUU3m/WqorSs9UgOHYm8Cd8rIDZsswDgYDVR0PAQH/BAQDAgEGMA8G
A1UdEwEB/wQFMAMBAf8wDQYJKoZIhvcNAQEMBQADggIBAFzUfA3P9wF9QZllDHPF
Up/L+M+ZBn8b2kMVn54CVVeWFPFSPCeHlCjtHzoBN6J2/FNQwISbxmtOuowhT6KO
VWKR82kV2LyI48SqC/3vqOlLVSoGIG1VeCkZ7l8wXEskEVX/JJpuXior7gtNn3/3
ATiUFJVDBwn7YKnuHKsSjKCaXqeYalltiz8I+8jRRa8YFWSQEg9zKC7F4iRO/Fjs
8PRF/iKz6y+O0tlFYQXBl2+odnKPi4w2r78NBc5xjeambx9spnFixdjQg3IM8WcR
iQycE0xyNN+81XHfqnHd4blsjDwSXWXavVcStkNr/+XeTWYRUc+ZruwXtuhxkYze
Sf7dNXGiFSeUHM9h4ya7b6NnJSFd5t0dCy5oGzuCr+yDZ4XUmFF0sbmZgIn/f3gZ
XHlKYC6SQK5MNyosycdiyA5d9zZbyuAlJQG03RoHnHcAP9Dc1ew91Pq7P8yF1m9/
qS3fuQL39ZeatTXaw2ewh0qpKJ4jjv9cJ2vhsE/zB+4ALtRZh8tSQZXq9EfX7mRB
VXyNWQKV3WKdwrnuWih0hKWbt5DHDAff9Yk2dDLWKMGwsAvgnEzDHNb842m1R0aB
L6KCq9NjRHDEjf8tM7qtj3u1cIiuPhnPQCjY/MiQu12ZIvVS5ljFH4gxQ+6IHdfG
jjxDah2nGN59PRbxYvnKkKj9
-----END CERTIFICATE-----
-----BEGIN CERTIFICATE-----
MIICjzCCAhWgAwIBAgIQXIuZxVqUxdJxVt7NiYDMJjAKBggqhkjOPQQDAzCBiDEL
MAkGA1UEBhMCVVMxEzARBgNVBAgTCk5ldyBKZXJzZXkxFDASBgNVBAcTC0plcnNl
eSBDaXR5MR4wHAYDVQQKExVUaGUgVVNFUlRSVVNUIE5ldHdvcmsxLjAsBgNVBAMT
JVVTRVJUcnVzdCBFQ0MgQ2VydGlmaWNhdGlvbiBBdXRob3JpdHkwHhcNMTAwMjAx
MDAwMDAwWhcNMzgwMTE4MjM1OTU5WjCBiDELMAkGA1UEBhMCVVMxEzARBgNVBAgT
Ck5ldyBKZXJzZXkxFDASBgNVBAcTC0plcnNleSBDaXR5MR4wHAYDVQQKExVUaGUg
VVNFUlRSVVNUIE5ldHdvcmsxLjAsBgNVBAMTJVVTRVJUcnVzdCBFQ0MgQ2VydGlm
aWNhdGlvbiBBdXRob3JpdHkwdjAQBgcqhkjOPQIBBgUrgQQAIgNiAAQarFRaqflo
I+d61SRvU8Za2EurxtW20eZzca7dnNYMYf3boIkDuAUU7FfO7l0/4iGzzvfUinng
o4N+LZfQYcTxmdwlkWOrfzCjtHDix6EznPO/LlxTsV+zfTJ/ijTjeXmjQjBAMB0G
A1UdDgQWBBQ64QmG1M8ZwpZ2dEl23OA1xmNjmjAOBgNVHQ8BAf8EBAMCAQYwDwYD
VR0TAQH/BAUwAwEB/zAKBggqhkjOPQQDAwNoADBlAjA2Z6EWCNzklwBBHU6+4WMB
zzuqQhFkoJ2UOQIReVx7Hfpkue4WQrO/isIJxOzksU0CMQDpKmFHjFJKS04YcPbW
RNZu9YO6bVi9JNlWSOrvxKJGgYhqOkbRqZtNyWHa0V1Xahg=
-----END CERTIFICATE-----
-----BEGIN CERTIFICATE-----
MIIDdTCCAl2gAwIBAgILBAAAAAABFUtaw5QwDQYJKoZIhvcNAQEFBQAwVzELMAkG
A1UEBhMCQkUxGTAXBgNVBAoTEEdsb2JhbFNpZ24gbnYtc2ExEDAOBgNVBAsTB1Jv
b3QgQ0ExGzAZBgNVBAMTEkdsb2JhbFNpZ24gUm9vdCBDQTAeFw05ODA5MDExMjAw
MDBaFw0yODAxMjgxMjAwMDBaMFcxCzAJBgNVBAYTAkJFMRkwFwYDVQQKExBHbG9i
YWxTaWduIG52LXNhMRAwDgYDVQQLEwdSb290IENBMRswGQYDVQQDExJHbG9iYWxT
aWduIFJvb3QgQ0EwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQDaDuaZ
jc6j40+Kfvvxi4Mla+pIH/EqsLmVEQS98GPR4mdmzxzdzxtIK+6NiY6arymAZavp
xy0Sy6scTHAHoT0KMM0VjU/43dSMUBUc71DuxC73/OlS8pF94G3VNTCOXkNz8kHp
1Wrjsok6Vjk4bwY8iGlbKk3Fp1S4bInMm/k8yuX9ifUSPJJ4ltbcdG6TRGHRjcdG
snUOhugZitVtbNV4FpWi6cgKOOvyJBNPc1STE4U6G7weNLWLBYy5d4ux2x8gkasJ
U26Qzns3dLlwR5EiUWMWea6xrkEmCMgZK9FGqkjWZCrXgzT/LCrBbBlDSgeF59N8
9iFo7+ryUp9/k5DPAgMBAAGjQjBAMA4GA1UdDwEB/wQEAwIBBjAPBgNVHRMBAf8E
BTADAQH/MB0GA1UdDgQWBBRge2YaRQ2XyolQL30EzTSo//z9SzANBgkqhkiG9w0B
AQUFAAOCAQEA1nPnfE920I2/7LqivjTFKDK1fPxsnCwrvQmeU79rXqoRSLblCKOz
yj1hTdNGCbM+w6DjY1Ub8rrvrTnhQ7k4o+YviiY776BQVvnGCv04zcQLcFGUl5gE
38NflNUVyRRBnMRddWQVDf9VMOyGj/8N7yy5Y0b2qvzfvGn9LhJIZJrglfCm7ymP
AbEVtQwdpf5pLGkkeB6zpxxxYu7KyJesF12KwvhHhm4qxFYxldBniYUr+WymXUad
DKqC5JlR3XC321Y9YeRq4VzW9v493kHMB65jUr9TU/Qr6cf9tveCX4XSQRjbgbME
HMUfpIBvFSDJ3gyICh3WZlXi/EjJKSZp4A==
-----END CERTIFICATE-----
-----BEGIN CERTIFICATE-----
MIIDXzCCAkegAwIBAgILBAAAAAABIVhTCKIwDQYJKoZIhvcNAQELBQAwTDEgMB4G
A1UECxMXR2xvYmFsU2lnbiBSb290IENBIC0gUjMxEzARBgNVBAoTCkdsb2JhbFNp
Z24xEzARBgNVBAMTCkdsb2JhbFNpZ24wHhcNMDkwMzE4MTAwMDAwWhcNMjkwMzE4
MTAwMDAwWjBMMSAwHgYDVQQLExdHbG9iYWxTaWduIFJvb3QgQ0EgLSBSMzETMBEG
A1UEChMKR2xvYmFsU2lnbjETMBEGA1UEAxMKR2xvYmFsU2lnbjCCASIwDQYJKoZI
hvcNAQEBBQADggEPADCCAQoCggEBAMwldpB5BngiFvXAg7aEyiie/QV2EcWtiHL8
RgJDx7KKnQRfJMsuS+FggkbhUqsMgUdwbN1k0ev1LKMPgj0MK66X17YUhhB5uzsT
gHeMCOFJ0mpiLx9e+pZo34knlTifBtc+ycsmWQ1z3rDI6SYOgxXG71uL0gRgykmm
KPZpO/bLyCiR5Z2KYVc3rHQU3HTgOu5yLy6c+9C7v/U9AOEGM+iCK65TpjoWc4zd
QQ4gOsC0p6Hpsk+QLjJg6VfLuQSSaGjlOCZgdbKfd/+RFO+uIEn8rUAVSNECMWEZ
XriX7613t2Saer9fwRPvm2L7DWzgVGkWqQPabumDk3F2xmmFghcCAwEAAaNCMEAw
DgYDVR0PAQH/BAQDAgEGMA8GA1UdEwEB/wQFMAMBAf8wHQYDVR0OBBYEFI/wS3+o
LkUkrk1Q+mOai97i3Ru8MA0GCSqGSIb3DQEBCwUAA4IBAQBLQNvAUKr+yAzv95ZU
RUm7lgAJQayzE4aGKAczymvmdLm6AC2upArT9fHxD4q/c2dKg8dEe3jgr25sbwMp
jjM5RcOO5LlXbKr8EpbsU8Yt5CRsuZRj+9xTaGdWPoO4zzUhw8lo/s7awlOqzJCK
6fBdRoyV3XpYKBovHd7NADdBj+1EbddTKJd+82cEHhXXipa0095MJ6RMG3NzdvQX
mcIfeg7jLQitChws/zyrVQ4PkX4268NXSb7hLi18YIvDQVETI53O9zJrlAGomecs
Mx86OyXShkDOOyyGeMlhLxS67ttVb9+E7gUJTb0o2HLO02JQZR7rkpeDMdmztcpH
WD9f
-----END CERTIFICATE-----
-----BEGIN CERTIFICATE-----
MIIFgzCCA2ugAwIBAgIORea7A4Mzw4VlSOb/RVEwDQYJKoZIhvcNAQEMBQAwTDEg
MB4GA1UECxMXR2xvYmFsU2lnbiBSb290IENBIC0gUjYxEzARBgNVBAoTCkdsb2Jh
bFNpZ24xEzARBgNVBAMTCkdsb2JhbFNpZ24wHhcNMTQxMjEwMDAwMDAwWhcNMzQx
MjEwMDAwMDAwWjBMMSAwHgYDVQQLExdHbG9iYWxTaWduIFJvb3QgQ0EgLSBSNjET
MBEGA1UEChMKR2xvYmFsU2lnbjETMBEGA1UEAxMKR2xvYmFsU2lnbjCCAiIwDQYJ
KoZIhvcNAQEBBQADggIPADCCAgoCggIBAJUH6HPKZvnsFMp7PPcNCPG0RQssgrRI
xutbPK6DuEGSMxSkb3/pKszGsIhrxbaJ0cay/xTOURQh7ErdG1rG1ofuTToVBu1k
ZguSgMpE3nOUTvOniX9PeGMIyBJQbUJmL025eShNUhqKGoC3GYEOfsSKvGRMIRxD
aNc9PIrFsmbVkJq3MQbFvuJtMgamHvm566qjuL++gmNQ0PAYid/kD3n16qIfKtJw
LnvnvJO7bVPiSHyMEAc4/2ayd2F+4OqMPKq0pPbzlUoSB239jLKJz9CgYXfIWHSw
1CM69106yqLbnQneXUQtkPGBzVeS+n68UARjNN9rkxi+azayOeSsJDa38O+2HBNX
k7besvjihbdzorg1qkXy4J02oW9UivFyVm4uiMVRQkQVlO6jxTiWm05OWgtH8wY2
SXcwvHE35absIQh1/OZhFj931dmRl4QKbNQCTXTAFO39OfuD8l4UoQSwC+n+7o/h
bguyCLNhZglqsQY6ZZZZwPA1/cnaKI0aEYdwgQqomnUdnjqGBQCe24DWJfncBZ4n
WUx2OVvq+aWh2IMP0f/fMBH5hc8zSPXKbWQULHpYT9NLCEnFlWQaYw55PfWzjMpY
rZxCRXluDocZXFSxZba/jJvcE+kNb7gu3GduyYsRtYQUigAZcIN5kZeR1Bonvzce
MgfYFGM8KEyvAgMBAAGjYzBhMA4GA1UdDwEB/wQEAwIBBjAPBgNVHRMBAf8EBTAD
AQH/MB0GA1UdDgQWBBSubAWjkxPioufi1xzWx/B/yGdToDAfBgNVHSMEGDAWgBSu
bAWjkxPioufi1xzWx/B/yGdToDANBgkqhkiG9w0BAQwFAAOCAgEAgyXt6NH9lVLN
nsAEoJFp5lzQhN7craJP6Ed41mWYqVuoPId8AorRbrcWc+ZfwFSY1XS+wc3iEZGt
Ixg93eFyRJa0lV7Ae46ZeBZDE1ZXs6KzO7V33EByrKPrmzU+sQghoefEQzd5Mr61
55wsTLxDKZmOMNOsIeDjHfrYBzN2VAAiKrlNIC5waNrlU/yDXNOd8v9EDERm8tLj
vUYAGm0CuiVdjaExUd1URhxN25mW7xocBFymFe944Hn+Xds+qkxV/ZoVqW/hpvvf
cDDpw+5CRu3CkwWJ+n1jez/QcYF8AOiYrg54NMMl+68KnyBr3TsTjxKM4kEaSHpz
oHdpx7Zcf4LIHv5YGygrqGytXm3ABdJ7t+uA/iU3/gKbaKxCXcPu9czc8FB10jZp
nOZ7BN9uBmm23goJSFmH63sUYHpkqmlD75HHTOwY3WzvUy2MmeFe8nI+z1TIvWfs
pA9MRf/TuTAjB0yPEL+GltmZWrSZVxykzLsViVO6LAUP5MSeGbEYNNVMnbrt9x+v
JJUEeKgDu+6B5dpffItKoZB0JaezPkvILFa9x8jvOOJckvB595yEunQtYQEgfn7R
8k8HWV+LLUNS60YMlOH1Zkd5d9VUWx+tJDfLRVpOoERIyNiwmcUVhAn21klJwGW4
5hpxbqCo8YLoRT5s1gLXCmeDBVrJpBA=
-----END CERTIFICATE-----
-----BEGIN CERTIFICATE-----
MIIDQTCCAimgAwIBAgITBmyfz5m/jAo54vB4ikPmljZbyjANBgkqhkiG9w0BAQsF
ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6
b24gUm9vdCBDQSAxMB4XDTE1MDUyNjAwMDAwMFoXDTM4MDExNzAwMDAwMFowOTEL
MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv
b3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj
ca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM
9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw
IFAGbHrQgLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWa6
VOujw5H5SNz/0egwLX0tdHA114gk957EWW67c4cX8jJGKLhD+rcdqsq08p8kDi1L
93FcXmn/6pUCyziKrlA4b9v7LWIbxcceVOF34GfID5yHI9Y/QCB/IIDEgEw+OyQm
jgSubJrIqg0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMC
AYYwHQYDVR0OBBYEFIQYzIU07LwMlJQuCFmcx7IQTgoIMA0GCSqGSIb3DQEBCwUA
A4IBAQCY8jdaQZChGsV2USggNiMOruYou6r4lK5IpDB/G/wkjUu0yKGX9rbxenDI
U5PMCCjjmCXPI6T53iHTfIUJrU6adTrCC2qJeHZERxhlbI1Bjjt/msv0tadQ1wUs
N+gDS63pYaACbvXy8MWy7Vu33PqUXHeeE6V/Uq2V8viTO96LXFvKWlJbYK8U90vv
o/ufQJVtMVT8QtPHRh8jrdkPSHCa2XV4cdFyQzR1bldZwgJcJmApzyMZFo6IQ6XU
5MsI+yMRQ+hDKXJioaldXgjUkK642M4UwtBV8ob2xJNDd2ZhwLnoQdeXeGADbkpy
rqXRfboQnoZsG4q5WTP468SQvvG5
-----END CERTIFICATE-----
-----BEGIN CERTIFICATE-----
MIIFQTCCAymgAwIBAgITBmyf0pY1hp8KD+WGePhbJruKNzANBgkqhkiG9w0BAQwF
ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6
b24gUm9vdCBDQSAyMB4XDTE1MDUyNjAwMDAwMFoXDTQwMDUyNjAwMDAwMFowOTEL
MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv
b3QgQ0EgMjCCAiIwDQYJKoZIhvcNAQEBBQADggIPADCCAgoCggIBAK2Wny2cSkxK
gXlRmeyKy2tgURO8TW0G/LAIjd0ZEGrHJgw12MBvIITplLGbhQPDW9tK6Mj4kHbZ
W0/jTOgGNk3Mmqw9DJArktQGGWCsN0R5hYGCrVo34A3MnaZMUnbqQ523BNFQ9lXg
1dKmSYXpN+nKfq5clU1Imj+uIFptiJXZNLhSGkOQsL9sBbm2eLfq0OQ6PBJTYv9K
8nu+NQWpEjTj82R0Yiw9AElaKP4yRLuH3WUnAnE72kr3H9rN9yFVkE8P7K6C4Z9r
2UXTu/Bfh+08LDmG2j/e7HJV63mjrdvdfLC6HM783k81ds8P+HgfajZRRidhW+me
z/CiVX18JYpvL7TFz4QuK/0NURBs+18bvBt+xa47mAExkv8LV/SasrlX6avvDXbR
8O70zoan4G7ptGmh32n2M8ZpLpcTnqWHsFcQgTfJU7O7f/aS0ZzQGPSSbtqDT6Zj
mUyl+17vIWR6IF9sZIUVyzfpYgwLKhbcAS4y2j5L9Z469hdAlO+ekQiG+r5jqFoz
7Mt0Q5X5bGlSNscpb/xVA1wf+5+9R+vnSUeVC06JIglJ4PVhHvG/LopyboBZ/1c6
+XUyo05f7O0oYtlNc/LMgRdg7c3r3NunysV+Ar3yVAhU/bQtCSwXVEqY0VThUWcI
0u1ufm8/0i2BWSlmy5A5lREedCf+3euvAgMBAAGjQjBAMA8GA1UdEwEB/wQFMAMB
Af8wDgYDVR0PAQH/BAQDAgGGMB0GA1UdDgQWBBSwDPBMMPQFWAJI/TPlUq9LhONm
UjANBgkqhkiG9w0BAQwFAAOCAgEAqqiAjw54o+Ci1M3m9Zh6O+oAA7CXDpO8Wqj2
LIxyh6mx/H9z/WNxeKWHWc8w4Q0QshNabYL1auaAn6AFC2jkR2vHat+2/XcycuUY
+gn0oJMsXdKMdYV2ZZAMA3m3MSNjrXiDCYZohMr/+c8mmpJ5581LxedhpxfL86kS
k5Nrp+gvU5LEYFiwzAJRGFuFjWJZY7attN6a+yb3ACfAXVU3dJnJUH/jWS5E4ywl
7uxMMne0nxrpS10gxdr9HIcWxkPo1LsmmkVwXqkLN1PiRnsn/eBG8om3zEK2yygm
btmlyTrIQRNg91CMFa6ybRoVGld45pIq2WWQgj9sAq+uEjonljYE1x2igGOpm/Hl
urR8FLBOybEfdF849lHqm/osohHUqS0nGkWxr7JOcQ3AWEbWaQbLU8uz/mtBzUF+
fUwPfHJ5elnNXkoOrJupmHN5fLT0zLm4BwyydFy4x2+IoZCn9Kr5v2c69BoVYh63
n749sSmvZ6ES8lgQGVMDMBu4Gon2nL2XA46jCfMdiyHxtN/kHNGfZQIG6lzWE7OE
76KlXIx3KadowGuuQNKotOrN8I1LOJwZmhsoVLiJkO/KdYE+HvJkJMcYr07/R54H
9jVlpNMKVv/1F2Rs76giJUmTtt8AF9pYfl3uxRuw0dFfIRDH+fO6AgonB8Xx1sfT
4PsJYGw=
-----END CERTIFICATE-----
-----BEGIN CERTIFICATE-----
MIIBtjCCAVugAwIBAgITBmyf1XSXNmY/Owua2eiedgPySjAKBggqhkjOPQQDAjA5
MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6b24g
Um9vdCBDQSAzMB4XDTE1MDUyNjAwMDAwMFoXDTQwMDUyNjAwMDAwMFowOTELMAkG
A1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJvb3Qg
Q0EgMzBZMBMGByqGSM49AgEGCCqGSM49AwEHA0IABCmXp8ZBf8ANm+gBG1bG8lKl
ui2yEujSLtf6ycXYqm0fc4E7O5hrOXwzpcVOho6AF2hiRVd9RFgdszflZwjrZt6j
QjBAMA8GA1UdEwEB/wQFMAMBAf8wDgYDVR0PAQH/BAQDAgGGMB0GA1UdDgQWBBSr
ttvXBp43rDCGB5Fwx5zEGbF4wDAKBggqhkjOPQQDAgNJADBGAiEA4IWSoxe3jfkr
BqWTrBqYaGFy+uGh0PsceGCmQ5nFuMQCIQCcAu/xlJyzlvnrxir4tiz+OpAUFteM
YyRIHN8wfdVoOw==
-----END CERTIFICATE-----
-----BEGIN CERTIFICATE-----
MIIB8jCCAXigAwIBAgITBmyf18G7EEwpQ+Vxe3ssyBrBDjAKBggqhkjOPQQDAzA5
MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6b24g
Um9vdCBDQSA0MB4XDTE1MDUyNjAwMDAwMFoXDTQwMDUyNjAwMDAwMFowOTELMAkG
A1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJvb3Qg
Q0EgNDB2MBAGByqGSM49AgEGBSuBBAAiA2IABNKrijdPo1MN/sGKe0uoe0ZLY7Bi
9i0b2whxIdIA6GO9mif78DluXeo9pcmBqqNbIJhFXRbb/egQbeOc4OO9X4Ri83Bk
M6DLJC9wuoihKqB1+IGuYgbEgds5bimwHvouXKNCMEAwDwYDVR0TAQH/BAUwAwEB
/zAOBgNVHQ8BAf8EBAMCAYYwHQYDVR0OBBYEFNPsxzplbszh2naaVvuc84ZtV+WB
MAoGCCqGSM49BAMDA2gAMGUCMDqLIfG9fhGt0O9Yli/W651+kI0rz2ZVwyzjKKlw
CkcO8DdZEv8tmZQoTipPNU0zWgIxAOp1AE47xDqUEpHJWEadIRNyp4iciuRMStuW
1KyLa2tJElMzrdfkviT8tQp21KW8EA==
-----END CERTIFICATE-----
-----BEGIN CERTIFICATE-----
MIIFijCCA3KgAwIBAgIQdY39i658BwD6qSWn4cetFDANBgkqhkiG9w0BAQwFADBf
MQswCQYDVQQGEwJHQjEYMBYGA1UEChMPU2VjdGlnbyBMaW1pdGVkMTYwNAYDVQQD
Ey1TZWN0aWdvIFB1YmxpYyBTZXJ2ZXIgQXV0aGVudGljYXRpb24gUm9vdCBSNDYw
HhcNMjEwMzIyMDAwMDAwWhcNNDYwMzIxMjM1OTU5WjBfMQswCQYDVQQGEwJHQjEY
MBYGA1UEChMPU2VjdGlnbyBMaW1pdGVkMTYwNAYDVQQDEy1TZWN0aWdvIFB1Ymxp
YyBTZXJ2ZXIgQXV0aGVudGljYXRpb24gUm9vdCBSNDYwggIiMA0GCSqGSIb3DQEB
AQUAA4ICDwAwggIKAoICAQCTvtU2UnXYASOgHEdCSe5jtrch/cSV1UgrJnwUUxDa
ef0rty2k1Cz66jLdScK5vQ9IPXtamFSvnl0xdE8H/FAh3aTPaE8bEmNtJZlMKpnz
SDBh+oF8HqcIStw+KxwfGExxqjWMrfhu6DtK2eWUAtaJhBOqbchPM8xQljeSM9xf
iOefVNlI8JhD1mb9nxc4Q8UBUQvX4yMPFF1bFOdLvt30yNoDN9HWOaEhUTCDsG3X
ME6WW5HwcCSrv0WBZEMNvSE6Lzzpng3LILVCJ8zab5vuZDCQOc2TZYEhMbUjUDM3
IuM47fgxMMxF/mL50V0yeUKH32rMVhlATc6qu/m1dkmU8Sf4kaWD5QazYw6A3OAS
VYCmO2a0OYctyPDQ0RTp5A1NDvZdV3LFOxxHVp3i1fuBYYzMTYCQNFu31xR13NgE
SJ/AwSiItOkcyqex8Va3e0lMWeUgFaiEAin6OJRpmkkGj80feRQXEgyDet4fsZfu
+Zd4KKTIRJLpfSYFplhym3kT2BFfrsU4YjRosoYwjviQYZ4ybPUHNs2iTG7sijbt
8uaZFURww3y8nDnAtOFr94MlI1fZEoDlSfB1D++N6xybVCi0ITz8fAr/73trdf+L
HaAZBav6+CuBQug4urv7qv094PPK306Xlynt8xhW6aWWrL3DkJiy4Pmi1KZHQ3xt
zwIDAQABo0IwQDAdBgNVHQ4EFgQUVnNYZJX5khqwEioEYnmhQBWIIUkwDgYDVR0P
AQH/BAQDAgGGMA8GA1UdEwEB/wQFMAMBAf8wDQYJKoZIhvcNAQEMBQADggIBAC9c
mTz8Bl6MlC5w6tIyMY208FHVvArzZJ8HXtXBc2hkeqK5Duj5XYUtqDdFqij0lgVQ
YKlJfp/imTYpE0RHap1VIDzYm/EDMrraQKFz6oOht0SmDpkBm+S8f74TlH7Kph52
gDY9hAaLMyZlbcp+nv4fjFg4exqDsQ+8FxG75gbMY/qB8oFM2gsQa6H61SilzwZA
Fv97fRheORKkU55+MkIQpiGRqRxOF3yEvJ+M0ejf5lG5Nkc/kLnHvALcWxxPDkjB
JYOcCj+esQMzEhonrPcibCTRAUH4WAP+JWgiH5paPHxsnnVI84HxZmduTILA7rpX
DhjvLpr3Etiga+kFpaHpaPi8TD8SHkXoUsCjvxInebnMMTzD9joiFgOgyY9mpFui
TdaBJQbpdqQACj7LzTWb4OE4y2BThihCQRxEV+ioratF4yUQvNs+ZUH7G6aXD+u5
dHn5HrwdVw1Hr8Mvn4dGp+smWg9WY7ViYG4A++MnESLn/pmPNPW56MORcr3Ywx65
LvKRRFHQV80MNNVIIb/bE/FmJUNS0nAiNs2fxBx1IK1jcmMGDw4nztJqDby1ORrp
0XZ60Vzk50lJLVU3aPAaOpg+VBeHVOmmJ1CJeyAvP/+/oYtKR5j/K3tJPsMpRmAY
QqszKbrAKbkTidOIijlBO8n9pu0f9GBj39ItVQGL
-----END CERTIFICATE-----
-----BEGIN CERTIFICATE-----
MIICOjCCAcGgAwIBAgIQQvLM2htpN0RfFf51KBC49DAKBggqhkjOPQQDAzBfMQsw
CQYDVQQGEwJHQjEYMBYGA1UEChMPU2VjdGlnbyBMaW1pdGVkMTYwNAYDVQQDEy1T
ZWN0aWdvIFB1YmxpYyBTZXJ2ZXIgQXV0aGVudGljYXRpb24gUm9vdCBFNDYwHhcN
MjEwMzIyMDAwMDAwWhcNNDYwMzIxMjM1OTU5WjBfMQswCQYDVQQGEwJHQjEYMBYG
A1UEChMPU2VjdGlnbyBMaW1pdGVkMTYwNAYDVQQDEy1TZWN0aWdvIFB1YmxpYyBT
ZXJ2ZXIgQXV0aGVudGljYXRpb24gUm9vdCBFNDYwdjAQBgcqhkjOPQIBBgUrgQQA
IgNiAAR2+pmpbiDt+dd34wc7qNs9Xzjoq1WmVk/WSOrsfy2qw7LFeeyZYX8QeccC
WvkEN/U0NSt3zn8gj1KjAIns1aeibVvjS5KToID1AZTc8GgHHs3u/iVStSBDHBv+
6xnOQ6OjQjBAMB0GA1UdDgQWBBTRItpMWfFLXyY4qp3W7usNw/upYTAOBgNVHQ8B
Af8EBAMCAYYwDwYDVR0TAQH/BAUwAwEB/zAKBggqhkjOPQQDAwNnADBkAjAn7qRa
qCG76UeXlImldCBteU/IvZNeWBj7LRoAasm4PdCkT0RHlAFWovgzJQxC36oCMB3q
4S6ILuH5px0CMk7yn2xVdOOurvulGu7t0vzCAxHrRVxgED1cf5kDW21USAGKcw==
-----END CERTIFICATE-----
-----BEGIN CERTIFICATE-----
MIID7zCCAtegAwIBAgIBADANBgkqhkiG9w0BAQsFADCBmDELMAkGA1UEBhMCVVMx
EDAOBgNVBAgTB0FyaXpvbmExEzARBgNVBAcTClNjb3R0c2RhbGUxJTAjBgNVBAoT
HFN0YXJmaWVsZCBUZWNobm9sb2dpZXMsIEluYy4xOzA5BgNVBAMTMlN0YXJmaWVs
ZCBTZXJ2aWNlcyBSb290IENlcnRpZmljYXRlIEF1dGhvcml0eSAtIEcyMB4XDTA5
MDkwMTAwMDAwMFoXDTM3MTIzMTIzNTk1OVowgZgxCzAJBgNVBAYTAlVTMRAwDgYD
VQQIEwdBcml6b25hMRMwEQYDVQQHEwpTY290dHNkYWxlMSUwIwYDVQQKExxTdGFy
ZmllbGQgVGVjaG5vbG9naWVzLCBJbmMuMTswOQYDVQQDEzJTdGFyZmllbGQgU2Vy
dmljZXMgUm9vdCBDZXJ0aWZpY2F0ZSBBdXRob3JpdHkgLSBHMjCCASIwDQYJKoZI
hvcNAQEBBQADggEPADCCAQoCggEBANUMOsQq+U7i9b4Zl1+OiFOxHz/Lz58gE20p
OsgPfTz3a3Y4Y9k2YKibXlwAgLIvWX/2h/klQ4bnaRtSmpDhcePYLQ1Ob/bISdm2
8xpWriu2dBTrz/sm4xq6HZYuajtYlIlHVv8loJNwU4PahHQUw2eeBGg6345AWh1K
Ts9DkTvnVtYAcMtS7nt9rjrnvDH5RfbCYM8TWQIrgMw0R9+53pBlbQLPLJGmpufe
hRhJfGZOozptqbXuNC66DQO4M99H67FrjSXZm86B0UVGMpZwh94CDklDhbZsc7tk
6mFBrMnUVN+HL8cisibMn1lUaJ/8viovxFUcdUBgF4UCVTmLfwUCAwEAAaNCMEAw
DwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMCAQYwHQYDVR0OBBYEFJxfAN+q
AdcwKziIorhtSpzyEZGDMA0GCSqGSIb3DQEBCwUAA4IBAQBLNqaEd2ndOxmfZyMI
bw5hyf2E3F/YNoHN2BtBLZ9g3ccaaNnRbobhiCPPE95Dz+I0swSdHynVv/heyNXB
ve6SbzJ08pGCL72CQnqtKrcgfU28elUSwhXqvfdqlS5sdJ/PHLTyxQGjhdByPq1z
qwubdQxtRbeOlKyWN7Wg0I8VRw7j6IPdj/3vQQF3zCepYoUz8jcI73HPdwbeyBkd
iEDPfUYd/x7H4c7/I9vG+o1VTqkC50cRRj70/b17KSa7qWFiNyi2LSr2EIZkyXCn
0q23KXB56jzaYyWf/Wi3MOxw+3WKt21gZ7IeyLnp2KhvAotnDU0mV3HaIPzBSlCN
sSi6
-----END CERTIFICATE-----
)PEM";

TFT_eSPI tft;
Preferences prefs;
WebServer server(80);
uint16_t calData[5]={251,3604,300,3459,7};
const int BL=27, CX=130,CY=160,RR=105;
const uint16_t BG=0x10A3,CARD=0x21A8,NAV=0x1967,ACCENT=0x6DDF,MUTED=0xBDF7,GRID=0x0326;
const int radii[]={5,10,25,50,100}, rates[]={15,30,60,120};
const char* modes[]={"Manual","Auto cycle","Radar","Details"};
const char* units[]={"Nautical miles","Miles","Kilometers"};
String ssid,pass,apPass;
float lat=40.6413,lon=-73.7781;
int radius=25,brightness=100,rate=60,mode=0,unit=0;
bool portal=false,online=false,held=false,follow=true,pwm=false;
unsigned long lastFetch=0,lastCycle=0,lastConnect=0;
unsigned int flightFailures=0;
unsigned long wifiLostAt=0;
unsigned long netFinished=0;
unsigned long retryDelay();
bool forceFetch=true;
enum Page { RADAR, FLIGHTS, DETAILS, WEATHER, SETTINGS, OPTIONS, NETWORKS, KEYBOARD, PORTAL, RESET_CONFIRM };
Page page=RADAR;
int option=0,offset=0,netOffset=0,netCount=0;
String networks[40];
String selectedHex,edit,editSSID,notice;
int editField=0,keyboardMode=0; bool showPassword=false;
float pendingLat=0;
struct Aircraft {String hex,call,type,reg; float lat,lon,alt,speed,heading,dist;};
Aircraft planes[80]; int count=0,selected=-1;
struct Detail {String key,airline,number,origin,dest,from,to,model,iata,photoURL; float fromLat=NAN,fromLon=NAN,toLat=NAN,toLon=NAN; unsigned long stamp=0; bool valid=false,full=false;};
Detail cache[12],currentFlightDetail; int cacheSlot=0;

struct ImageCache {
  String key, credit;
  bool reference=false, referenceTried=false;
  std::unique_ptr<uint8_t[]> bytes;
  size_t size=0;
  unsigned long tried=0;
};
ImageCache photo,logo;
struct Station {String id,name; float la=0,lo=0;};
struct WeatherData {String id,name,category,raw,wind,visibility,observation; float temp=NAN,dew=NAN,pressure=NAN; bool valid=false;};
struct Place {String name; float la=0,lo=0;};
struct MapSegment {float a,b,c,d; bool water;};
MapSegment mapLines[240]; int mapLineCount=0;
Station stations[20]; int stationCount=0; Place places[24]; int placeCount=0;
WeatherData wx;
String weatherAirport="AUTO";
int radarStyle=0,boardStyle=0,settingsPage=0,utcOffset=0;
bool nightDim=false,flyover=true,photosEnabled=true,zoneKnown=false;
unsigned long lastWeather=0,lastGeo=0,lastZone=0,lastClockDraw=0,lastGood=0;
bool detailWanted=true,imagesWanted=true,geoWanted=true,weatherWanted=true,zoneWanted=true,networkBusy=false;
uint32_t generation=0;
String imageWantedKey="";
QueueHandle_t workQ=nullptr,resultQ=nullptr;
TaskHandle_t workerHandle=nullptr;
enum JobKind { J_FLIGHTS,J_DETAIL,J_IMAGES,J_WEATHER,J_GEO,J_ZONE,J_BOARD,J_MODEL };
struct Job {JobKind kind; uint32_t gen; float la,lo; int radius; Aircraft aircraft; Detail info; String airport,logoKey,photoKey; bool wantLogo=false,wantPhoto=false,referenceOnly=false;};
struct Result {JobKind kind; uint32_t gen; bool ok=false; Aircraft *list=nullptr; int n=0; Detail info; ImageCache logo,photo; bool didLogo=false,didPhoto=false; WeatherData wx; Station stations[20]; int stationCount=0; Place places[24]; int placeCount=0; MapSegment lines[240]; int lineCount=0; int utcOffset=0; ~Result(){delete[] list;}};
void scheduleNetwork(); void pollNetwork(); void drawWeather(); void networkWorker(void*);
// Explicit prototypes prevent Arduino's generated prototypes from preceding custom types.
Detail fetchDetail(Aircraft &a, bool full);
Detail fetchModel(Aircraft &a, Detail d);
void fetchFlightJob(Job &j, Result &res);
String logoCode(const Aircraft &a, const Detail &d);
void fetchImages(Job &j, Result &r);
bool fetchReferencePhoto(Job &j, ImageCache &dst);
String referenceCategory(const Aircraft &a,const Detail &d);
String urlEncode(String input);
void fetchWeather(Job &j, Result &r);
void fetchGeo(Job &j, Result &r);
void fetchZone(Job &j, Result &r);

TFT_eSprite radarSprite(&tft);
bool spriteReady=false;
unsigned long lastFrame=0;
int sweep=0;
struct Trail { String hex; float la[8],lo[8]; uint8_t n=0; unsigned long stamp=0; };
Trail trails[80];
unsigned long retryDelay(){return min(60000UL,15000UL*(unsigned long)max(1u,flightFailures));}
void releaseRadar(){if(spriteReady){radarSprite.deleteSprite();spriteReady=false;}}
void recordTrails(){
  for(int i=0;i<count;i++){
    if(!planes[i].hex.length())continue;
    int slot=-1,old=0;
    for(int j=0;j<80;j++){
      if(trails[j].hex==planes[i].hex){slot=j;break;}
      if(!trails[j].n || millis()-trails[j].stamp>millis()-trails[old].stamp)old=j;
    }
    if(slot<0){slot=old;trails[slot].hex=planes[i].hex;trails[slot].n=0;}
    Trail &t=trails[slot];
    if(t.n==8){for(int j=1;j<8;j++){t.la[j-1]=t.la[j];t.lo[j-1]=t.lo[j];}t.n--;}
    t.la[t.n]=planes[i].lat;t.lo[t.n]=planes[i].lon;t.n++;t.stamp=millis();
  }
}

bool imageDownload(String url,ImageCache &dst,size_t cap);
bool retryImage(ImageCache &im,String key);
bool request(String url,JsonDocument &doc,JsonDocument *filter=nullptr);
void textAt(String s,int x,int y,int size,uint16_t color,int width=450);
void btn(int x,int y,int w,int h,String label,uint16_t color=CARD);
void draw();
void startPortal();
void stopPortal();
void connectSaved();
void scanNetworks();
void updateFlights();
void loadDetails();

bool validChoice(int v,const int* a,int n){for(int i=0;i<n;i++)if(v==a[i])return true;return false;}
String clockText(){time_t now=time(nullptr);if(now<1700000000)return "--:--";now+=utcOffset;struct tm tmv;gmtime_r(&now,&tmv);char b[16];strftime(b,sizeof(b),"%H:%M",&tmv);return String(b)+(zoneKnown?"":" UTC");}
void applyBrightness(){int b=brightness;if(nightDim&&zoneKnown&&time(nullptr)>1700000000){time_t now=time(nullptr)+utcOffset;struct tm t;gmtime_r(&now,&t);if(t.tm_hour>=22||t.tm_hour<7)b=min(b,20);}if(pwm)ledcWrite(BL,b*255/100);}
void saveSettings(){prefs.putString("ssid",ssid);prefs.putString("pass",pass);prefs.putFloat("lat",lat);prefs.putFloat("lon",lon);prefs.putInt("radius",radius);prefs.putInt("bright",brightness);prefs.putInt("rate",rate);prefs.putInt("mode",mode);prefs.putInt("unit",unit);prefs.putString("wxAirport",weatherAirport);prefs.putInt("radStyle",radarStyle);prefs.putInt("boardStyle",boardStyle);prefs.putBool("nightDim",nightDim);prefs.putBool("flyover",flyover);prefs.putBool("photos",photosEnabled);}
void loadSettings(){ssid=prefs.getString("ssid","");pass=prefs.getString("pass","");lat=prefs.getFloat("lat",40.6413);lon=prefs.getFloat("lon",-73.7781);radius=prefs.getInt("radius",25);brightness=constrain(prefs.getInt("bright",100),10,100);rate=prefs.getInt("rate",60);if(!prefs.getBool("stable733",false)){rate=max(rate,60);prefs.putInt("rate",rate);prefs.putBool("stable733",true);}mode=constrain(prefs.getInt("mode",0),0,3);unit=constrain(prefs.getInt("unit",0),0,2);if(!validChoice(radius,radii,5))radius=25;if(!validChoice(rate,rates,4))rate=60;if(!isfinite(lat)||fabs(lat)>90)lat=40.6413;if(!isfinite(lon)||fabs(lon)>180)lon=-73.7781;weatherAirport=prefs.getString("wxAirport","AUTO");radarStyle=constrain(prefs.getInt("radStyle",0),0,1);boardStyle=constrain(prefs.getInt("boardStyle",0),0,1);nightDim=prefs.getBool("nightDim",false);flyover=prefs.getBool("flyover",true);photosEnabled=prefs.getBool("photos",true);}
String fit(String s,int w){if(tft.textWidth(s)<=w)return s;while(s.length()&&tft.textWidth(s+"...")>w)s.remove(s.length()-1);return s+"...";}
void textAt(String s,int x,int y,int size,uint16_t color,int width){tft.setTextSize(size);tft.setTextColor(color);tft.drawString(fit(s,width),x,y);}
void btn(int x,int y,int w,int h,String label,uint16_t color){tft.fillRoundRect(x,y,w,h,4,color);int size=((h>=39 && w>=140)||label.length()==1)?2:1;tft.setTextSize(size);if(tft.textWidth(label)>w-8){size=1;tft.setTextSize(1);}tft.setTextColor(TFT_WHITE);tft.drawCentreString(fit(label,w-6),x+w/2,y+(h-8*size)/2,1);}
void drawStatusHeader(){
 tft.fillRect(306,0,174,35,NAV);
 tft.setTextFont(1);tft.setTextSize(1);tft.setTextColor(MUTED,NAV);
 tft.drawString(clockText(),310,8,1);
 if(lastGood){unsigned long age=(millis()-lastGood)/1000; tft.drawString(age>999?"999+s":String(age)+"s",374,21,1);}
 String status=WiFi.status()!=WL_CONNECTED?"NO WIFI":time(nullptr)<1700000000?"SYNC":flightFailures?(lastGood?"STALE":"RETRY"):networkBusy?"FETCH":online?"LIVE":"OFFLINE";
 tft.setTextColor(WiFi.status()!=WL_CONNECTED?TFT_RED:(networkBusy||flightFailures)?TFT_YELLOW:online?TFT_GREEN:TFT_RED,NAV);tft.drawString(status,424,8,1);
}
void header(String s){tft.fillScreen(BG);tft.fillRect(0,0,480,35,NAV);textAt(s,12,10,2,TFT_WHITE,290);drawStatusHeader();}
void nav(){
 const char* n[]={"RADAR","FLIGHTS","DETAILS","WEATHER","SETTINGS"};
 tft.fillRect(0,286,480,34,NAV);
 for(int i=0;i<5;i++){int x=i*96;uint16_t c=page==i?ACCENT:MUTED;
 if(page==i){tft.fillRoundRect(x+4,288,88,28,8,CARD);tft.fillRect(x+26,286,44,2,ACCENT);}
 tft.setTextSize(1);tft.setTextColor(c);
#ifdef LOAD_FONT2
 tft.drawCentreString(n[i],x+48,295,2);
#else
 tft.drawCentreString(n[i],x+48,300,1);
#endif
 }
}
void back(){btn(8,286,464,34,"BACK");}
String distanceText(float d){return String(d*(unit==1?1.150779:unit==2?1.852:1),1)+(unit==1?" mi":unit==2?" km":" NM");}
String speedText(float s){if(s<0)return "N/A";return String(s*(unit==1?1.150779:unit==2?1.852:1),0)+(unit==1?" mph":unit==2?" km/h":" kt");}
String altText(float a){if(a<0)return "N/A";return unit==2?String(a*0.3048,0)+" m":String(a,0)+" ft";}
float distanceNM(float a,float b){float da=radians(a-lat),db=radians(b-lon);float h=sin(da/2)*sin(da/2)+cos(radians(lat))*cos(radians(a))*sin(db/2)*sin(db/2);return 6880.13*asin(sqrt(constrain(h,0.0f,1.0f)));}
String modelName(String t){const char* codes[]={"B738","B737","B739","B38M","B39M","A319","A320","A321","A20N","A21N","B77W","B788","B789","CRJ9","CRJ7","E75L","E75S","E190"};const char* names[]={"Boeing 737-800","Boeing 737-700","Boeing 737-900","Boeing 737 MAX 8","Boeing 737 MAX 9","Airbus A319","Airbus A320","Airbus A321","Airbus A320neo","Airbus A321neo","Boeing 777-300ER","Boeing 787-8","Boeing 787-9","Bombardier CRJ900","Bombardier CRJ700","Embraer E175","Embraer E175","Embraer E190"};for(int i=0;i<18;i++)if(t==codes[i])return names[i];return t;}

class BoundedJsonBody : public Stream {
public:
  String body; bool overflow=false; size_t limit;
  explicit BoundedJsonBody(size_t cap):limit(cap){}
  int available() override{return 0;}
  int read() override{return -1;}
  int peek() override{return -1;}
  void flush() override{}
  size_t write(uint8_t b) override{return write(&b,1);}
  size_t write(const uint8_t *b,size_t n) override{
    if(n>limit-body.length()||!body.concat(reinterpret_cast<const char*>(b),(unsigned int)n)){overflow=true;setWriteError();return 0;}return n;
  }
};
bool request(String url,JsonDocument &doc,JsonDocument *filter){
  if(WiFi.status()!=WL_CONNECTED){Serial.println("Request skipped: WiFi disconnected");return false;}
  WiFiClientSecure client;client.setCACert(ROOTS);HTTPClient http;
  http.useHTTP10(false);http.setTimeout(15000);http.setConnectTimeout(10000);client.setHandshakeTimeout(15);
  if(!http.begin(client,url)){Serial.println("HTTP initialization failed: "+url);return false;}
  if(url.startsWith("https://commons.wikimedia.org/"))http.setUserAgent("FlightRadar/7.3.5 (personal ESP32 aircraft display)");
  const char* headers[]={"Transfer-Encoding"};http.collectHeaders(headers,1);
  int code=http.GET();Serial.printf("HTTP %d %s\n",code,url.c_str());
  if(code!=200){if(code<0){char tlsError[160]={0};int tlsCode=client.lastError(tlsError,sizeof(tlsError));Serial.printf("HTTP error: %s; TLS error %d: %s; heap %u largest %u\n",HTTPClient::errorToString(code).c_str(),tlsCode,tlsError,ESP.getFreeHeap(),ESP.getMaxAllocHeap());}http.end();return false;}
  String encoding=http.header("Transfer-Encoding");encoding.toLowerCase();
  // Raw HTTPClient streams do not decode chunked framing. Use its body decoder
  // for chunked responses, while preserving low-memory streaming otherwise.
  DeserializationError err;
  if(filter&&encoding.indexOf("chunked")<0){
    err=deserializeJson(doc,http.getStream(),DeserializationOption::Filter(*filter));http.end();
  }else{
    const size_t cap=filter?96000:64000;
    int length=http.getSize();if(length>(int)cap){Serial.println("JSON response exceeds memory budget");http.end();return false;}
    BoundedJsonBody sink(cap);
    int received=http.writeToStream(&sink);http.end();
    if(received<0||sink.overflow||(size_t)received!=sink.body.length()||(length>=0&&received!=length)){Serial.printf("JSON body failed: %d, buffered %u, heap %u\n",received,(unsigned)sink.body.length(),ESP.getFreeHeap());return false;}
    if(filter)err=deserializeJson(doc,sink.body,DeserializationOption::Filter(*filter));else err=deserializeJson(doc,sink.body);
  }
  if(err){Serial.printf("JSON error: %s; transfer=%s; heap=%u\n",err.c_str(),encoding.c_str(),ESP.getFreeHeap());return false;}return true;
}
String airport(JsonVariant v){String s=v["iata_code"]|"";return s.length()?s:String(v["icao_code"]|"---");}
Detail fetchDetail(Aircraft &a,bool full){String key=a.hex+":"+a.call;Detail d;d.key=key;d.airline="Unknown airline";d.number=a.call;d.origin=d.dest="---";d.from=d.to="Unknown";d.model=modelName(a.type);bool ok=a.call.length()&&a.call!=a.hex;for(unsigned i=0;i<a.call.length();i++)if(!isalnum((unsigned char)a.call[i]))ok=false;if(ok){JsonDocument doc;if(request("https://api.adsbdb.com/v0/callsign/"+a.call,doc)){JsonVariant r=doc["response"]["flightroute"];d.airline=r["airline"]["name"]|"Unknown airline";d.iata=r["airline"]["iata"]|"";String n=r["callsign_iata"]|"";if(n.length())d.number=n;d.origin=airport(r["origin"]);d.dest=airport(r["destination"]);d.from=r["origin"]["municipality"]|"Unknown";d.to=r["destination"]["municipality"]|"Unknown";d.fromLat=r["origin"]["latitude"]|NAN;d.fromLon=r["origin"]["longitude"]|NAN;d.toLat=r["destination"]["latitude"]|NAN;d.toLon=r["destination"]["longitude"]|NAN;}}d.valid=true;d.full=false;d.stamp=millis();return d;}
Detail fetchModel(Aircraft &a,Detail d){bool ok=a.hex.length()==6;for(unsigned i=0;i<a.hex.length();i++)if(!isxdigit((unsigned char)a.hex[i]))ok=false;if(ok){JsonDocument doc;if(request("https://api.adsbdb.com/v0/aircraft/"+a.hex,doc)){JsonVariant r=doc["response"]["aircraft"];String m=r["type"]|"", maker=r["manufacturer"]|"";d.photoURL=r["url_photo_thumbnail"]|"";if(m.length())d.model=maker.length()&&!m.startsWith(maker)?maker+" "+m:m;}}d.full=true;return d;}
void loadDetails(){currentFlightDetail=Detail();if(selected<0||selected>=count){detailWanted=false;return;}Aircraft &a=planes[selected];String key=a.hex+":"+a.call;for(int i=0;i<12;i++)if(cache[i].valid&&cache[i].key==key&&millis()-cache[i].stamp<600000){currentFlightDetail=cache[i];detailWanted=false;imagesWanted=true;return;}currentFlightDetail.key=key;currentFlightDetail.airline="Fetching route...";currentFlightDetail.number=a.call;currentFlightDetail.origin=currentFlightDetail.dest="---";currentFlightDetail.model=modelName(a.type);detailWanted=true;imagesWanted=true;}
float rangeNM(float la,float lo,float a,float b){float da=radians(a-la),db=radians(b-lo);float h=sin(da/2)*sin(da/2)+cos(radians(la))*cos(radians(a))*sin(db/2)*sin(db/2);return 6880.13*asin(sqrt(constrain(h,0.0f,1.0f)));}
void fetchFlightJob(Job &j,Result &res){JsonDocument f,doc;const char* keys[]={"flight","hex","lat","lon","alt_baro","gs","track","t","r"};for(auto k:keys)f["ac"][0][k]=true;String url="https://api.adsb.lol/v2/lat/"+String(j.la,5)+"/lon/"+String(j.lo,5)+"/dist/"+String(j.radius);res.ok=request(url,doc,&f)&&doc["ac"].is<JsonArray>();if(!res.ok)return;res.list=new(std::nothrow) Aircraft[80];if(!res.list){res.ok=false;return;}res.n=0;for(JsonObject p:doc["ac"].as<JsonArray>()){if(p["lat"].isNull()||p["lon"].isNull()||p["alt_baro"]=="ground")continue;Aircraft a;a.lat=p["lat"].as<float>();a.lon=p["lon"].as<float>();a.dist=rangeNM(j.la,j.lo,a.lat,a.lon);if(a.dist>j.radius)continue;a.hex=p["hex"]|"";a.call=p["flight"]|"";a.call.trim();if(!a.call.length())a.call=a.hex.length()?a.hex:"UNKNOWN";a.type=p["t"]|"Unknown";a.reg=p["r"]|"Unknown";a.alt=p["alt_baro"]|-1.0f;a.speed=p["gs"]|-1.0f;a.heading=p["track"]|0.0f;int pos=res.n; if(res.n==80){if(a.dist>=res.list[79].dist)continue;pos=79;}else res.n++;while(pos>0&&res.list[pos-1].dist>a.dist){res.list[pos]=res.list[pos-1];pos--;}res.list[pos]=a;}}
void updateFlights(){forceFetch=true;}
void position(int i,int &x,int &y){float east=(fmodf(planes[i].lon-lon+540,360)-180)*60*cos(radians(lat));float north=(planes[i].lat-lat)*60;x=CX+east*RR/radius;y=CY-north*RR/radius;}
void radarLabel(int i,int &lx,int &ly,int &w){int x,y;position(i,x,y);w=6*planes[i].call.length();lx=x+12;if(lx+w>260)lx=x-w-12;lx=max(0,lx);ly=constrain(y-10,37,263);}
bool visiblePlane(int i){int x,y;position(i,x,y);return sq(x-CX)+sq(y-CY)<=sq(RR);}
// Palette-aware drawing surface, with no framebuffer needed for the fallback.
struct RadarSurface {
  bool buffered; const uint16_t *pal;
  void fillSprite(int c){if(buffered)radarSprite.fillSprite(c);else tft.fillRect(0,0,264,251,pal[c]);}
  void setTextSize(int n){if(buffered)radarSprite.setTextSize(n);else tft.setTextSize(n);}
  void setTextColor(int c){if(buffered)radarSprite.setTextColor(c);else tft.setTextColor(pal[c]);}
  void drawString(const String &v,int x,int y){if(buffered)radarSprite.drawString(v,x,y);else tft.drawString(v,x,y,1);}
  void drawLine(int x,int y,int a,int b,int c){if(buffered)radarSprite.drawLine(x,y,a,b,c);else tft.drawLine(x,y,a,b,pal[c]);}
  void drawRect(int x,int y,int w,int h,int c){if(buffered)radarSprite.drawRect(x,y,w,h,c);else tft.drawRect(x,y,w,h,pal[c]);}
  void fillRect(int x,int y,int w,int h,int c){if(buffered)radarSprite.fillRect(x,y,w,h,c);else tft.fillRect(x,y,w,h,pal[c]);}
  void drawCircle(int x,int y,int r,int c){if(buffered)radarSprite.drawCircle(x,y,r,c);else tft.drawCircle(x,y,r,pal[c]);}
  void fillCircle(int x,int y,int r,int c){if(buffered)radarSprite.fillCircle(x,y,r,c);else tft.fillCircle(x,y,r,pal[c]);}
  void drawFastHLine(int x,int y,int w,int c){if(buffered)radarSprite.drawFastHLine(x,y,w,c);else tft.drawFastHLine(x,y,w,pal[c]);}
  void drawFastVLine(int x,int y,int h,int c){if(buffered)radarSprite.drawFastVLine(x,y,h,c);else tft.drawFastVLine(x,y,h,pal[c]);}
  void fillTriangle(int x,int y,int a,int b,int d,int e,int c){if(buffered)radarSprite.fillTriangle(x,y,a,b,d,e,c);else tft.fillTriangle(x,y,a,b,d,e,pal[c]);}
};
void drawRadarFrame(){
  uint16_t pal[]={TFT_BLACK,0x0120,0x0240,0x03e0,TFT_GREEN,TFT_YELLOW,TFT_WHITE,0x07e8,0x0520,0x04a0,0x0320,0x0200,0x0140,0x0660,0x7bef,0x0861};
  if(radarStyle){pal[2]=0x4208;pal[3]=0x4A69;pal[4]=0x9EFF;pal[5]=0xFDC0;pal[7]=0x8D5B;pal[9]=0xCE88;pal[12]=0x39AB;pal[13]=0x3374;pal[14]=0xB5D5;pal[15]=0x1947;}
  if(!networkBusy&&!spriteReady&&ESP.getMaxAllocHeap()>42000){radarSprite.setColorDepth(4);spriteReady=radarSprite.createSprite(264,251)!=nullptr;if(spriteReady)for(int i=0;i<16;i++)radarSprite.setPaletteColor(i,pal[i]);}
  bool buffered=spriteReady&&!networkBusy;
  if(!buffered){tft.setViewport(0,35,264,251,true);tft.setTextFont(1);tft.setTextDatum(TL_DATUM);}
  RadarSurface r{buffered,pal};const int cy=CY-35;
  r.fillSprite(radarStyle?15:0);r.setTextSize(1);
  if(radarStyle){
    float scale=60.0f*RR/radius, cs=cos(radians(lat));
    for(int i=0;i<mapLineCount;i++){auto &m=mapLines[i];int x=CX+(m.b-lon)*scale*cs,y=cy-(m.a-lat)*scale,xx=CX+(m.d-lon)*scale*cs,yy=cy-(m.c-lat)*scale;if(x<0||x>=264||xx<0||xx>=264||y<0||y>=238||yy<0||yy>=238)continue;r.drawLine(x,y,xx,yy,m.water?13:12);if(m.water)r.drawLine(x+1,y,xx+1,yy,13);}
    int usedX[10],usedY[10],usedN=0;
    for(int i=0;i<placeCount&&usedN<5;i++){int x=CX+(places[i].lo-lon)*scale*cs,y=cy-(places[i].la-lat)*scale;String name=places[i].name.substring(0,13);if(x<5||x+6*(int)name.length()>259||y<12||y>225)continue;bool near=false;for(int k=0;k<usedN;k++)if(abs(x-usedX[k])<68&&abs(y-usedY[k])<20)near=true;if(near)continue;r.setTextColor(14);r.drawString(name,x,y);usedX[usedN]=x;usedY[usedN++]=y;}
    for(int i=0;i<stationCount&&i<5;i++){int x=CX+(stations[i].lo-lon)*scale*cs,y=cy-(stations[i].la-lat)*scale;if(sq(x-CX)+sq(y-cy)>sq(RR))continue;bool near=false;for(int k=0;k<usedN;k++)if(abs(x-usedX[k])<42&&abs(y-usedY[k])<14)near=true;r.drawRect(x-2,y-2,5,5,7);if(!near&&x<230){r.setTextColor(7);r.drawString(stations[i].id,x+5,y+3);usedX[usedN]=x;usedY[usedN++]=y;}}
  }

  // Narrow fading sector. Repaint only the radar, not the details panel.
  if(!radarStyle)for(int a=30;a>=0;a-=2){float h=radians(sweep-a);r.drawLine(CX,cy,CX+sin(h)*RR,cy-cos(h)*RR,a<4?13:a<12?3:a<22?2:1);}
  for(int i=1;i<=4;i++)if(!radarStyle||i==2||i==4)r.drawCircle(CX,cy,RR*i/4,2);
  if(!radarStyle){r.drawFastHLine(CX-RR,cy,RR*2,2);r.drawFastVLine(CX,cy-RR,RR*2,2);}
  r.setTextSize(1);r.setTextColor(14);r.drawString("N",127,8);r.drawString("S",127,231);r.drawString("W",9,121);r.drawString("E",245,121);
  r.fillCircle(CX,cy,3,6);
  for(int i=count-1;i>=0;i--){if(!visiblePlane(i))continue;
    for(int j=0;j<80;j++)if(trails[j].n&&trails[j].hex==planes[i].hex){Trail &t=trails[j];int px=0,py=0;bool prev=false;for(int k=0;k<t.n;k++){int x=CX+(t.lo[k]-lon)*60*cos(radians(lat))*RR/radius;int y=cy-(t.la[k]-lat)*60*RR/radius;bool inside=sq(x-CX)+sq(y-cy)<=sq(RR);if(inside&&prev)r.drawLine(px,py,x,y,i==selected?9:3);prev=inside;px=x;py=y;}break;}
    int x,y;position(i,x,y);y-=35;int c=i==selected?5:4;float a=radians(planes[i].heading);float sn=sin(a),cs=cos(a);
    // Aircraft silhouette: fuselage, wings, and tail, rotated with heading.
    const int vx[]={0,-2,-11,-11,-2,-2,-5,-5,0,5,5,2,2,11,11,2};
    const int vy[]={-12,-3,3,5,2,8,11,13,10,13,11,8,2,5,3,-3};
    int xx[16],yy[16];for(int k=0;k<16;k++){xx[k]=x+vx[k]*cs-vy[k]*sn;yy[k]=y+vx[k]*sn+vy[k]*cs;}
    for(int k=0;k<16;k++)r.fillTriangle(x,y,xx[k],yy[k],xx[(k+1)%16],yy[(k+1)%16],c);
    if(i==selected)r.drawCircle(x,y,16,5);
    int lx,ly,w;radarLabel(i,lx,ly,w);if(!radarStyle||i==selected||i<4){r.setTextColor(c);r.drawString(planes[i].call,lx,ly-35);}
  }
  r.fillRect(0,240,264,11,radarStyle?15:0);r.setTextColor(14);r.drawString(String(count)+" aircraft",5,242);if(radarStyle)r.drawString("(c) OSM",105,242);String rangeLabel=distanceText(radius);r.drawString(rangeLabel,260-6*rangeLabel.length(),242);
  if(buffered)radarSprite.pushSprite(0,35);else tft.resetViewport();lastFrame=millis();
}
String flyoverText(){if(!flyover||!online||millis()-lastGood>rate*3000UL)return "";float soon=999;String call;for(int i=0;i<count;i++){Aircraft &a=planes[i];if(a.speed<60)continue;float x=(a.lon-lon)*60*cos(radians(lat)),y=(a.lat-lat)*60;float h=radians(a.heading),vx=sin(h)*a.speed/60,vy=cos(h)*a.speed/60;float t=-(x*vx+y*vy)/(vx*vx+vy*vy);if(t>=0&&t<=5&&t<soon&&hypotf(x+vx*t,y+vy*t)<1){soon=t;call=a.call;}}return call.length()?call+" near "+String(soon,1)+"m":"";}
void drawRadar(){String alert=flyoverText();header(alert.length()?alert:"FLIGHT RADAR");drawRadarFrame();tft.fillRoundRect(269,40,207,244,8,CARD);textAt(follow?"NEAREST AIRCRAFT":"SELECTED AIRCRAFT",278,45,1,MUTED,195);if(selected>=0){Aircraft &a=planes[selected];textAt(a.call,278,62,3,TFT_YELLOW,195);textAt(currentFlightDetail.airline,278,92,1,ACCENT,195);textAt(currentFlightDetail.origin+" > "+currentFlightDetail.dest,278,111,2,TFT_WHITE,195);textAt(currentFlightDetail.from,278,135,1,MUTED,195);textAt(currentFlightDetail.to,278,148,1,MUTED,195);textAt(currentFlightDetail.model,278,169,1,ACCENT,195);textAt(altText(a.alt),278,189,2,TFT_WHITE,195);textAt(speedText(a.speed),278,212,2,TFT_WHITE,195);textAt(distanceText(a.dist),327,239,1,TFT_WHITE,96);}else textAt("No aircraft",278,95,2,TFT_WHITE);btn(270,257,100,25,"NEAREST");btn(374,257,102,25,"DETAILS");btn(270,232,55,23,"PREV");btn(425,232,51,23,"NEXT");nav();}
void drawList(){header(boardStyle?"AIR TRAFFIC BOARD":"NEARBY FLIGHTS");textAt("CALLSIGN",14,44,1,MUTED);textAt(boardStyle?"ROUTE":"ALTITUDE",180,44,1,MUTED);textAt("DISTANCE",335,44,1,MUTED);for(int r=0;r<5&&offset+r<count;r++){int i=offset+r,y=59+r*38;Aircraft &a=planes[i];tft.fillRoundRect(8,y,464,34,4,CARD);if(i==selected)tft.drawRoundRect(8,y,464,34,4,TFT_YELLOW);textAt(a.call,17,y+9,2,i==selected?TFT_YELLOW:TFT_WHITE,155);String middle=altText(a.alt);if(boardStyle){middle="--- > ---";String key=a.hex+":"+a.call;for(auto &d:cache)if(d.valid&&d.key==key){middle=d.origin+" > "+d.dest;break;}}textAt(middle,180,y+9,2,boardStyle?TFT_YELLOW:TFT_WHITE,145);textAt(distanceText(a.dist),335,y+9,2,ACCENT,130);}if(!count)textAt("No nearby airborne aircraft",20,100,2,TFT_WHITE);btn(8,253,138,28,"PREVIOUS");btn(334,253,138,28,"NEXT");textAt(String(offset/5+1)+" / "+String(max(1,(count+4)/5)),210,260,2,MUTED);nav();}
// Bounded compressed image cache; no SD or full-screen framebuffer.
class ImageSink : public Stream {
public:
  uint8_t *data; size_t cap, used=0; bool overflow=false;
  ImageSink(uint8_t *d,size_t c):data(d),cap(c){}
  int available() override{return 0;}
  int read() override{return -1;}
  int peek() override{return -1;}
  void flush() override{}
  size_t write(uint8_t b) override{return write(&b,1);}
  size_t write(const uint8_t *b,size_t n) override{
    if(n>cap-used){overflow=true;setWriteError();return 0;}
    memcpy(data+used,b,n);used+=n;return n;
  }
};
bool imageDownload(String url,ImageCache &dst,size_t cap){
  if(!url.startsWith("https://")||WiFi.status()!=WL_CONNECTED)return false;
  
  WiFiClientSecure client;client.setCACert(ROOTS);HTTPClient http;
  http.setTimeout(6000);http.setConnectTimeout(4000);http.useHTTP10(false);
  if(!http.begin(client,url))return false;
  http.setUserAgent(url.startsWith("https://upload.wikimedia.org/")?"FlightRadar/7.3.5 (personal ESP32 aircraft display)":"FlightRadar/7.2 ESP32");
  http.setFollowRedirects(HTTPC_DISABLE_FOLLOW_REDIRECTS);
  int code=http.GET();int length=http.getSize();
  Serial.printf("Image HTTP %d length %d %s\n",code,length,url.c_str());
  if(code!=200||length>(int)cap){http.end();return false;}
  size_t alloc=length>0?(size_t)length:cap;
  std::unique_ptr<uint8_t[]> b(new(std::nothrow) uint8_t[alloc]);
  if(!b){Serial.printf("Image allocation failed: %u bytes, heap %u\n",(unsigned)alloc,ESP.getFreeHeap());http.end();return false;}
  ImageSink sink(b.get(),alloc);int received=http.writeToStream(&sink);http.end();
  if(received<=0||sink.overflow||sink.used!=(size_t)received||(length>=0&&received!=length)){Serial.printf("Image stream failed: %d bytes %u overflow %d\n",received,(unsigned)sink.used,sink.overflow);return false;}
  // Shrink to the actual compressed length where possible.
  std::unique_ptr<uint8_t[]> exact(alloc==sink.used?nullptr:new(std::nothrow) uint8_t[sink.used]);
  if(exact){memcpy(exact.get(),b.get(),sink.used);dst.bytes=std::move(exact);}else dst.bytes=std::move(b);
  dst.size=sink.used;Serial.printf("Image cached: %u bytes, heap %u\n",(unsigned)dst.size,ESP.getFreeHeap());return true;
}
bool retryImage(ImageCache &im,String key){
  if(im.key==key&&(im.size||millis()-im.tried<300000UL))return false;
  im.bytes.reset();im.size=0;im.credit="";im.key=key;im.tried=millis();return true;
}
String logoCode(const Aircraft &a,const Detail &d){String code=d.iata;if(code.length()!=2&&d.number!=a.call&&d.number.length()>2){String n=d.number;if(isalnum((unsigned char)n[0])&&isalnum((unsigned char)n[1])&&isdigit((unsigned char)n[2]))code=n.substring(0,2);}code.toUpperCase();if(code.length()!=2)return "-";for(unsigned i=0;i<code.length();i++)if(!isalnum((unsigned char)code[i]))return "-";return code;}
// Strict model + operator categories: no generic or unrelated airline photos.
String referenceCategory(const Aircraft &a,const Detail &d){
  String model=modelName(a.type);
  if(model==a.type||!a.type.length())return "";
  if(a.type=="B38M")model="Boeing 737-8 MAX";
  if(a.type=="B39M")model="Boeing 737-9 MAX";
  if(a.type=="E75L"||a.type=="E75S")model="Embraer 175";
  if(a.type=="E190")model="Embraer 190";
  if(a.type=="CRJ9")model="Bombardier CRJ900";
  String airline=d.airline;
  String op=a.call.substring(0,3);
  if(op=="AAL")airline="American Airlines";
  else if(op=="UAL")airline="United Airlines";
  else if(op=="DAL")airline="Delta Air Lines";
  else if(op=="SWA")airline="Southwest Airlines";
  else if(op=="RPA")airline="Republic Airline";
  else if(op=="ENY")airline="Envoy Air";
  else if(op=="AAY")airline="Allegiant Air";
  else if(op=="JBU")airline="JetBlue Airways";
  else if(op=="FFT")airline="Frontier Airlines";
  else if(op=="NKS")airline="Spirit Airlines";
  else if(op=="ASA")airline="Alaska Airlines";
  airline.trim();
  if(!airline.length()||airline=="Unknown airline"||airline=="Fetching route..."||airline.length()>70||airline.indexOf('"')>=0)return "";
  return model+" of "+airline;
}
String plainPhotoCredit(String in){
  String out;bool tag=false;
  for(unsigned i=0;i<in.length();i++){char c=in[i];if(c=='<'){tag=true;continue;}if(c=='>'){tag=false;continue;}if(!tag&&c>=32)out+=c;}
  out.replace("&amp;","&");out.replace("&quot;","\"");out.trim();return out;
}
bool fetchReferencePhoto(Job &j,ImageCache &dst){
  String category=referenceCategory(j.aircraft,j.info);if(!category.length())return false;
  String query="deepcategory:\""+category+"\" filetype:bitmap -intitle:interior -intitle:cockpit -intitle:cabin -intitle:seat -intitle:engine -intitle:winglet -intitle:logo -intitle:diagram";
  String url="https://commons.wikimedia.org/w/api.php?action=query&format=json&formatversion=2&generator=search&gsrnamespace=6&gsrlimit=3&gsrsearch="+urlEncode(query)+"&prop=imageinfo&iiprop=url%7Cmime%7Cextmetadata&iiurlwidth=320&iiextmetadatafilter=Artist%7CLicenseShortName%7CLicenseUrl%7CAttributionRequired";
  String thumb,author,license,source,licenseUrl,title;
  {
    JsonDocument filter,doc;
    filter["query"]["pages"][0]["title"]=true;
    JsonObject ii=filter["query"]["pages"][0]["imageinfo"][0].to<JsonObject>();
    ii["thumburl"]=true;ii["descriptionurl"]=true;ii["mime"]=true;
    for(const char* k:{"Artist","LicenseShortName","LicenseUrl","AttributionRequired"})ii["extmetadata"][k]["value"]=true;
    Serial.println("Reference photo category: "+category);
    if(!request(url,doc,&filter))return false;
    for(JsonObject p:doc["query"]["pages"].as<JsonArray>()){
      JsonObject im=p["imageinfo"][0].as<JsonObject>();
      String u=im["thumburl"]|"",mime=im["mime"]|"";
      String l=im["extmetadata"]["LicenseShortName"]["value"]|"";
      String a=plainPhotoCredit(im["extmetadata"]["Artist"]["value"]|"");
      String page=im["descriptionurl"]|"";
      if(mime!="image/jpeg"||!u.startsWith("https://upload.wikimedia.org/")||!page.startsWith("https://commons.wikimedia.org/"))continue;
      if(!(l.startsWith("CC BY")||l=="CC0"||l=="Public domain")||(!a.length()&&l.startsWith("CC BY")))continue;
      if(l.startsWith("CC BY")&&String(im["extmetadata"]["LicenseUrl"]["value"]|"").length()==0)continue;
      thumb=u;author=a;license=l;source=page;licenseUrl=im["extmetadata"]["LicenseUrl"]["value"]|"";title=p["title"]|"";break;
    }
  } // Free metadata before second TLS connection.
  if(!thumb.length())return false;
  if(!imageDownload(thumb,dst,32000))return false;
  if(dst.size<3||dst.bytes[0]!=0xff||dst.bytes[1]!=0xd8){dst.bytes.reset();dst.size=0;return false;}
  dst.reference=true;dst.credit=license+(author.length()?String(" / ")+author:String(""));
  Serial.println("REFERENCE PHOTO (not exact airframe): "+title+" | "+category);
  Serial.println("Author: "+author+" | License: "+license+" | "+licenseUrl);
  Serial.println("Source: "+source+" | resized thumbnail");return true;
}
void fetchImages(Job &j,Result &r){
 if(j.wantLogo){r.didLogo=true;r.logo.key=j.logoKey;r.logo.tried=millis();if(j.logoKey!="-")imageDownload("https://cdn.jsdelivr.net/gh/spydogenesis/airlines-logo@main/airlines-logo/200x200_v2/"+j.logoKey+".png",r.logo,24000);}
 if(j.wantPhoto){r.didPhoto=true;r.photo.key=j.photoKey;r.photo.tried=millis();
  if(j.referenceOnly){r.photo.referenceTried=true;fetchReferencePhoto(j,r.photo);return;}
  String url=j.info.photoURL;
  if(!url.length()){Serial.println("ADSBDB has no thumbnail for "+j.aircraft.call);}
  else {Serial.println("ADSBDB thumbnail: "+url);r.photo.credit=url.indexOf("airport-data.com/")>=0?"Airport-Data / ADSBDB":"ADSBDB thumbnail";if(!imageDownload(url,r.photo,48000))Serial.println("ADSBDB thumbnail failed");}
  if(r.photo.size&&(r.photo.size<3||r.photo.bytes[0]!=0xff||r.photo.bytes[1]!=0xd8)){r.photo.bytes.reset();r.photo.size=0;}
 }
}


String urlEncode(String input){String out;const char* h="0123456789ABCDEF";for(unsigned i=0;i<input.length();i++){unsigned char c=input[i];if(isalnum(c)||c=='-'||c=='_'||c=='.'||c=='~')out+=(char)c;else{out+='%';out+=h[c>>4];out+=h[c&15];}}return out;}
String metarBox(float la,float lo,float span){float dl=min(20.0f,span/max(0.1f,(float)cos(radians(la))));return String(max(-90.0f,la-span),3)+","+String(max(-180.0f,lo-dl),3)+","+String(min(90.0f,la+span),3)+","+String(min(180.0f,lo+dl),3);}
String jsonText(JsonVariant v){if(v.isNull())return "N/A";if(v.is<const char*>())return v.as<String>();String s;serializeJson(v,s);return s;}
void fetchWeather(Job &j,Result &r){JsonDocument doc;String base="https://aviationweather.gov/api/data/metar?format=json&hours=2";String url=base+"&bbox="+metarBox(j.la,j.lo,1.0f);bool got=request(url,doc)&&doc.is<JsonArray>();float best=1e9;JsonObject chosen;
 if(got){for(JsonObject p:doc.as<JsonArray>()){if(p["lat"].isNull()||p["lon"].isNull())continue;float a=p["lat"].as<float>(),b=p["lon"].as<float>(),d=rangeNM(j.la,j.lo,a,b);String id=p["icaoId"]|"";if(!id.length())continue;bool exists=false;for(int i=0;i<r.stationCount;i++)if(r.stations[i].id==id)exists=true;if(!exists){int pos=r.stationCount;if(pos==20){if(d>=rangeNM(j.la,j.lo,r.stations[19].la,r.stations[19].lo))pos=-1;else pos=19;}else r.stationCount++;if(pos>=0){while(pos>0&&rangeNM(j.la,j.lo,r.stations[pos-1].la,r.stations[pos-1].lo)>d){r.stations[pos]=r.stations[pos-1];pos--;}r.stations[pos]={id,String(p["name"]|""),a,b};}}
 if(d<best){best=d;chosen=p;}}}
 if(j.airport!="AUTO"){doc.clear();chosen=JsonObject();if(request(base+"&ids="+urlEncode(j.airport),doc)&&doc.is<JsonArray>()&&doc.size())chosen=doc[0].as<JsonObject>();}
 if(chosen.isNull()){r.ok=got;return;}WeatherData &w=r.wx;w.id=chosen["icaoId"]|"";w.name=chosen["name"]|"";w.category=chosen["fltCat"]|"N/A";w.raw=chosen["rawOb"]|"";w.temp=chosen["temp"]|NAN;w.dew=chosen["dewp"]|NAN;w.pressure=chosen["altim"]|NAN;w.wind=jsonText(chosen["wdir"])+" / "+jsonText(chosen["wspd"])+" kt";w.visibility=jsonText(chosen["visib"])+" SM";w.observation=jsonText(chosen["reportTime"]);w.valid=w.id.length()>0;r.ok=true;
}
void fetchGeo(Job &j,Result &r){
 float span=min(1.7f,max(0.2f,j.radius/60.0f));String bbox=metarBox(j.la,j.lo,span);
 String q="[out:json][timeout:8];node[place~\"^(city|town)$\"][name]("+bbox+");out 40;";
 JsonDocument doc;
 if(request("https://overpass-api.de/api/interpreter?data="+urlEncode(q),doc)){
 for(JsonObject p:doc["elements"].as<JsonArray>()){if(p["lat"].isNull()||p["lon"].isNull())continue;String name=p["tags"]["name"]|"";if(!name.length())continue;float a=p["lat"],b=p["lon"],d=rangeNM(j.la,j.lo,a,b);int pos=r.placeCount;if(pos==24){if(d>=rangeNM(j.la,j.lo,r.places[23].la,r.places[23].lo))continue;pos=23;}else r.placeCount++;while(pos>0&&rangeNM(j.la,j.lo,r.places[pos-1].la,r.places[pos-1].lo)>d){r.places[pos]=r.places[pos-1];pos--;}r.places[pos]={name.substring(0,24),a,b};}r.ok=true;}
 doc.clear();
 // Small bounded selection of real river and motorway geometry; no raster tiles.
 q="[out:json][timeout:8];(way[waterway=river]("+bbox+");way[highway=motorway]("+bbox+"););out geom("+bbox+") 18;";
 if(!request("https://overpass-api.de/api/interpreter?data="+urlEncode(q),doc))return;
 for(JsonObject e:doc["elements"].as<JsonArray>()){bool water=!e["tags"]["waterway"].isNull();float a=0,b=0;bool prev=false;for(JsonVariant v:e["geometry"].as<JsonArray>()){if(v["lat"].isNull()||v["lon"].isNull()){prev=false;continue;}float c=v["lat"],d=v["lon"];if(prev&&r.lineCount<240){if(rangeNM(j.la,j.lo,a,b)<=j.radius*1.5f&&rangeNM(j.la,j.lo,c,d)<=j.radius*1.5f)r.lines[r.lineCount++]={a,b,c,d,water};}a=c;b=d;prev=true;}}r.ok=true;
}
void fetchZone(Job &j,Result &r){JsonDocument doc;String u="https://api.open-meteo.com/v1/forecast?latitude="+String(j.la,5)+"&longitude="+String(j.lo,5)+"&current=temperature_2m&timezone=auto&forecast_days=1";if(request(u,doc)&&!doc["utc_offset_seconds"].isNull()){r.utcOffset=doc["utc_offset_seconds"].as<int>();r.ok=true;}}
void networkWorker(void*){for(;;){Job *j=nullptr;if(xQueueReceive(workQ,&j,portMAX_DELAY)!=pdTRUE||!j)continue;Result *r=new(std::nothrow) Result();if(r){r->kind=j->kind;r->gen=j->gen;if(WiFi.status()==WL_CONNECTED&&time(nullptr)>1700000000){switch(j->kind){case J_FLIGHTS:fetchFlightJob(*j,*r);break;case J_DETAIL:r->info=fetchDetail(j->aircraft,false);r->ok=true;break;case J_MODEL:r->info=fetchModel(j->aircraft,j->info);r->ok=true;break;case J_IMAGES:fetchImages(*j,*r);r->ok=true;break;case J_WEATHER:fetchWeather(*j,*r);break;case J_GEO:fetchGeo(*j,*r);break;case J_ZONE:fetchZone(*j,*r);break;case J_BOARD:r->info=fetchDetail(j->aircraft,false);r->ok=true;break;}}}delete j;xQueueSend(resultQ,&r,portMAX_DELAY);}}
void scheduleNetwork(){if(networkBusy||!workQ||!workerHandle||portal||WiFi.status()!=WL_CONNECTED||page>SETTINGS)return;if(time(nullptr)<1700000000)return;JobKind k;int boardIndex=-1;unsigned long now=millis();if(now-netFinished<1200UL)return;if(forceFetch||(flightFailures&&now-lastFetch>=retryDelay())||(!lastGood&&!flightFailures))k=J_FLIGHTS;else if(flightFailures)return;else if(now-lastFetch>=rate*1000UL)k=J_FLIGHTS;else if(!zoneKnown&&(zoneWanted||now-lastZone>=30000UL))k=J_ZONE;else if(detailWanted&&selected>=0)k=J_DETAIL;else if(page==DETAILS&&selected>=0&&currentFlightDetail.valid&&!currentFlightDetail.full)k=J_MODEL;else if(page==DETAILS&&selected>=0&&currentFlightDetail.valid&&imagesWanted)k=J_IMAGES;else if(weatherWanted||now-lastWeather>=300000UL)k=J_WEATHER;else if(zoneWanted||now-lastZone>=1800000UL)k=J_ZONE;else if(radarStyle==1&&(geoWanted||now-lastGeo>=1800000UL))k=J_GEO;else if(page==FLIGHTS&&boardStyle){for(int i=offset;i<count&&i<offset+5;i++){String key=planes[i].hex+":"+planes[i].call;bool cached=false;for(auto &d:cache)if(d.valid&&d.key==key&&now-d.stamp<600000UL)cached=true;if(!cached){boardIndex=i;break;}}if(boardIndex<0)return;k=J_BOARD;}else return;
 Job *j=new(std::nothrow) Job();if(!j)return;j->kind=k;j->gen=generation;j->la=lat;j->lo=lon;j->radius=radius;j->airport=weatherAirport;if(selected>=0){j->aircraft=planes[selected];j->info=currentFlightDetail;}
 if(k==J_BOARD)j->aircraft=planes[boardIndex];
 if(k==J_IMAGES){j->logoKey=logoCode(j->aircraft,j->info);j->photoKey=j->aircraft.hex;j->wantLogo=logo.key!=j->logoKey||(!logo.size&&now-logo.tried>=300000UL);j->referenceOnly=photo.key==j->photoKey&&!photo.size&&!photo.referenceTried&&now-photo.tried<300000UL; j->wantPhoto=photosEnabled&&j->photoKey.length()==6&&(photo.key!=j->photoKey||(!photo.size&&(j->referenceOnly||now-photo.tried>=300000UL)));if(!j->wantLogo&&!j->wantPhoto){imagesWanted=false;delete j;return;}if(j->wantPhoto){photo.bytes.reset();photo.size=0;}if(j->wantLogo){logo.bytes.reset();logo.size=0;}}
 releaseRadar();networkBusy=true;Serial.printf("Starting job %d: heap=%u largest=%u\n",(int)k,ESP.getFreeHeap(),ESP.getMaxAllocHeap());if(xQueueSend(workQ,&j,0)!=pdTRUE){networkBusy=false;delete j;return;}if(k==J_FLIGHTS){forceFetch=false;lastFetch=now;}if(k==J_DETAIL)detailWanted=false;if(k==J_IMAGES)imagesWanted=false;if(k==J_WEATHER){lastWeather=now;weatherWanted=false;}if(k==J_GEO){lastGeo=now;geoWanted=false;}if(k==J_ZONE){lastZone=now;zoneWanted=false;}}
void pollNetwork(){Result *r=nullptr;if(!resultQ||xQueueReceive(resultQ,&r,0)!=pdTRUE)return;networkBusy=false;netFinished=millis();bool repaint=false;if(!r){Serial.println("Network result allocation failed");forceFetch=true;return;}if(r->gen!=generation){delete r;return;}switch(r->kind){case J_FLIGHTS:lastFetch=millis();if(r->ok){online=true;flightFailures=0;String oldKey=currentFlightDetail.key;count=r->n;for(int i=0;i<count;i++)planes[i]=std::move(r->list[i]);selected=-1;if(!follow&&selectedHex.length())for(int i=0;i<count;i++)if(planes[i].hex==selectedHex){selected=i;break;}if(selected<0){follow=true;selected=count?0:-1;}selectedHex=selected>=0?planes[selected].hex:"";offset=constrain(offset,0,max(0,(count-1)/5*5));recordTrails();lastGood=millis();String key=selected>=0?planes[selected].hex+":"+planes[selected].call:"";if(key!=oldKey||(currentFlightDetail.valid&&millis()-currentFlightDetail.stamp>=600000UL))loadDetails();Serial.printf("Airborne: %d, heap: %u\n",count,ESP.getFreeHeap());}else{if(flightFailures<6)flightFailures++;online=false;Serial.printf("Flight refresh failed (%u); WiFi status=%d RSSI=%d heap=%u largest=%u; retry in %lu ms\n",flightFailures,(int)WiFi.status(),WiFi.RSSI(),ESP.getFreeHeap(),ESP.getMaxAllocHeap(),retryDelay());}repaint=true;break;
 case J_MODEL:case J_DETAIL:if(r->ok){int slot=-1;for(int i=0;i<12;i++)if(cache[i].valid&&cache[i].key==r->info.key){slot=i;break;}if(slot<0){slot=cacheSlot;cacheSlot=(cacheSlot+1)%12;}cache[slot]=r->info;if(selected>=0&&r->info.key==planes[selected].hex+":"+planes[selected].call){currentFlightDetail=r->info;detailWanted=false;imagesWanted=true;repaint=true;}}else if(selected>=0){currentFlightDetail.airline="Details unavailable";repaint=true;}break;
 case J_IMAGES:if(selected>=0){if(r->didLogo&&r->logo.key==logoCode(planes[selected],currentFlightDetail))logo=std::move(r->logo);if(r->didPhoto&&r->photo.key==planes[selected].hex){photo=std::move(r->photo);if(!photo.size&&!photo.referenceTried&&photosEnabled)imagesWanted=true;}repaint=page==DETAILS;}break;
 case J_WEATHER:if(r->ok){wx=r->wx;stationCount=r->stationCount;for(int i=0;i<stationCount;i++)stations[i]=r->stations[i];}else wx.valid=false;repaint=page==WEATHER||page==RADAR;break;
 case J_GEO:if(r->ok){placeCount=r->placeCount;for(int i=0;i<placeCount;i++)places[i]=r->places[i];mapLineCount=r->lineCount;for(int i=0;i<mapLineCount;i++)mapLines[i]=r->lines[i];}repaint=page==RADAR;break;
 case J_ZONE:if(r->ok){utcOffset=r->utcOffset;zoneKnown=true;Serial.printf("Time zone offset: %d seconds\n",utcOffset);applyBrightness();}break;case J_BOARD:if(r->ok){cache[cacheSlot]=r->info;cacheSlot=(cacheSlot+1)%12;repaint=page==FLIGHTS;}break;}
 delete r;if((repaint||page==RADAR)&&page<=WEATHER&&!held)draw();}

int imgX,imgY,imgRight,imgBottom;
bool jpgBlock(int16_t x,int16_t y,uint16_t w,uint16_t h,uint16_t *data){
  if(x>=imgRight||y>=imgBottom)return true;
  if(x+w<=imgRight&&y+h<=imgBottom)tft.pushImage(x,y,w,h,data);
  return true;
}
bool drawPhoto(int x,int y,int w,int h){
  if(!photo.bytes||photo.size<3)return false; if(photo.bytes[0]!=0xff||photo.bytes[1]!=0xd8){Serial.printf("Photo is not JPEG: %02x %02x\n",photo.bytes[0],photo.bytes[1]);return false;}
  uint16_t iw=0,ih=0;int sz=TJpgDec.getJpgSize(&iw,&ih,photo.bytes.get(),photo.size);Serial.printf("JPEG size result %d: %u x %u\n",sz,iw,ih);if(sz!=0||!iw||!ih)return false;
  int scale=1;while(scale<8&&((iw+scale-1)/scale>w||(ih+scale-1)/scale>h))scale*=2;
  if((iw+scale-1)/scale>w||(ih+scale-1)/scale>h)return false;
  imgRight=x+w;imgBottom=y+h;TJpgDec.setJpgScale(scale);TJpgDec.setCallback(jpgBlock);TJpgDec.setSwapBytes(false);
  bool swap=tft.getSwapBytes();tft.setSwapBytes(true);
  int result=TJpgDec.drawJpg(x+(w-iw/scale)/2,y+(h-ih/scale)/2,photo.bytes.get(),photo.size);Serial.printf("JPEG decode result: %d scale %d\n",result,scale);bool ok=result==0;
  tft.setSwapBytes(swap);return ok;
}
PNG *pngDecoder=nullptr;
int logoX,logoY,logoW,logoH,logoSrcW,logoSrcH,logoNext;
int logoLine(PNGDRAW *line){
  if(line->iWidth>256)return 0;
  uint16_t src[256],out[56];pngDecoder->getLineAsRGB565(line,src,PNG_RGB565_LITTLE_ENDIAN,0xffffffff);
  while(logoNext<logoH&&logoNext*logoSrcH/logoH==line->y){for(int i=0;i<logoW;i++)out[i]=src[i*logoSrcW/logoW];tft.pushImage(logoX,logoY+logoNext,logoW,1,out);logoNext++;}return 1;
}
bool drawLogo(int x,int y){
  if(!logo.bytes||logo.size<8||ESP.getFreeHeap()<sizeof(PNG)+18000)return false;
  std::unique_ptr<PNG> decoder(new(std::nothrow) PNG());if(!decoder)return false;pngDecoder=decoder.get();
  if(decoder->openRAM(logo.bytes.get(),logo.size,logoLine)!=PNG_SUCCESS){pngDecoder=nullptr;return false;}
  logoSrcW=decoder->getWidth();logoSrcH=decoder->getHeight();bool ok=false;
  if(logoSrcW>0&&logoSrcW<=256&&logoSrcH>0&&logoSrcH<=256){logoW=56;logoH=max(1,56*logoSrcH/logoSrcW);if(logoH>56){logoH=56;logoW=max(1,56*logoSrcW/logoSrcH);}logoX=x+(56-logoW)/2;logoY=y+(56-logoH)/2;logoNext=0;bool swap=tft.getSwapBytes();tft.setSwapBytes(true);ok=decoder->decode(nullptr,0)==PNG_SUCCESS;tft.setSwapBytes(swap);}
  decoder->close();pngDecoder=nullptr;return ok;
}
void drawDetails(){
  header("FLIGHT DETAILS");if(selected<0){textAt("No aircraft selected",15,100,2,TFT_WHITE);nav();return;}
  Aircraft &a=planes[selected];
  tft.fillRoundRect(10,40,460,65,5,CARD);
  tft.fillRoundRect(15,44,56,56,4,TFT_WHITE);
  if(logo.key!=logoCode(a,currentFlightDetail)||!drawLogo(15,44)){tft.fillRoundRect(15,44,56,56,4,NAV);textAt(a.call.substring(0,3),21,64,2,ACCENT,50);}
  textAt(currentFlightDetail.airline,82,49,2,TFT_WHITE,378);
  textAt(currentFlightDetail.number+" / "+a.call,82,77,2,ACCENT,378);
  tft.fillRoundRect(10,111,224,139,5,CARD);
  textAt(currentFlightDetail.origin+" > "+currentFlightDetail.dest,20,120,2,TFT_WHITE,204);
  textAt(currentFlightDetail.from+" > "+currentFlightDetail.to,20,143,1,MUTED,204);
  textAt("ALT  "+altText(a.alt),20,165,2,TFT_WHITE,204);
  textAt("SPD  "+speedText(a.speed),20,190,2,TFT_WHITE,204);
  textAt("DIST "+distanceText(a.dist),20,215,2,ACCENT,204);
  tft.fillRoundRect(240,111,230,139,5,CARD);
  bool ok=photosEnabled&&photo.key==a.hex&&drawPhoto(244,114,222,photo.reference?105:115);
  if(ok&&photo.reference){textAt("REFERENCE - same operator/model",245,221,1,ACCENT,220);textAt(photo.credit,245,237,1,MUTED,220);}
  else if(ok)textAt("Source: "+photo.credit,245,232,1,MUTED,220);
  else {if(photosEnabled&&photo.key==a.hex&&photo.size){photo.bytes.reset();photo.size=0;if(!photo.referenceTried){photo.tried=millis();imagesWanted=true;}}textAt(!photosEnabled?"Photographs disabled":networkBusy?"Loading / unavailable":"Aircraft photo unavailable",249,154,1,MUTED,216);textAt(a.type,249,175,2,ACCENT,216);}
  textAt(currentFlightDetail.model,10,255,2,TFT_WHITE,460);
  textAt("Reg: "+a.reg+"  Heading: "+String(a.heading,0),10,275,1,MUTED,460);nav();
}

void drawWeather(){
  header("AVIATION WEATHER");
  if(!wx.valid){
    tft.fillRoundRect(10,48,460,190,10,CARD);
    tft.drawCircle(240,100,17,ACCENT);tft.drawLine(240,100,240,89,ACCENT);tft.drawLine(240,100,248,105,ACCENT);
    textAt(networkBusy?"Fetching METAR...":"No METAR available",25,137,2,TFT_WHITE,430);
    textAt("Choose a reporting airport in Settings",25,168,1,MUTED,430);nav();return;
  }
  uint16_t cat=wx.category=="VFR"?0x66D0:wx.category=="MVFR"?0x7CFF:wx.category=="IFR"?0xFBAE:wx.category=="LIFR"?0xF8DF:MUTED;
  String categoryName=wx.category=="VFR"?"Visual":wx.category=="MVFR"?"Marginal visual":wx.category=="IFR"?"Instrument":wx.category=="LIFR"?"Low instrument":"Not reported";
  String temp=isnan(wx.temp)?String("N/A"):String(unit==2?wx.temp:wx.temp*1.8f+32,1)+(unit==2?" C":" F");
  String dew=isnan(wx.dew)?String("N/A"):String(unit==2?wx.dew:wx.dew*1.8f+32,1)+(unit==2?" C":" F");
  textAt(wx.id,12,43,2,ACCENT,85);textAt(wx.name,96,48,1,MUTED,370);
  tft.fillRoundRect(10,66,224,77,9,CARD);tft.fillRoundRect(240,66,230,77,9,CARD);
  textAt("TEMPERATURE",22,76,1,MUTED);textAt(temp,22,97,3,TFT_WHITE,200);
  textAt("FLIGHT CATEGORY",252,76,1,MUTED);textAt(wx.category,252,94,3,cat,145);textAt(categoryName,252,125,1,MUTED,200);
  tft.fillCircle(445,91,6,cat);
  tft.fillRoundRect(10,150,224,53,8,CARD);tft.fillRoundRect(240,150,230,53,8,CARD);
  tft.fillRoundRect(10,209,224,53,8,CARD);tft.fillRoundRect(240,209,230,53,8,CARD);
  textAt("WIND (FROM / SPEED)",20,158,1,MUTED,204);textAt(wx.wind,20,176,2,ACCENT,204);
  textAt("VISIBILITY",250,158,1,MUTED);textAt(wx.visibility,250,176,2,TFT_WHITE,210);
  textAt("ALTIMETER",20,217,1,MUTED);textAt(isnan(wx.pressure)?String("N/A"):String(wx.pressure*0.02953f,2)+" inHg",20,235,2,TFT_WHITE,204);
  textAt("DEW POINT",250,217,1,MUTED);textAt(dew,250,235,2,TFT_WHITE,210);
  textAt("METAR: "+wx.observation,12,266,1,MUTED,456);
  textAt("NOAA Aviation Weather Center",12,276,1,MUTED,456);nav();
}
void drawSettings(){header(settingsPage?"SETTINGS 2/2":"SETTINGS 1/2");const char* names[]={"WiFi Network","Location","Radar Radius","Brightness","Refresh Interval","Display Mode","Units","Weather Airport","Radar Style","Flight Board","Night Dimming","Flyover Alerts","Aircraft Photos","Factory Reset"};String vals[]={ssid.length()?ssid:"Not configured",String(lat,4)+", "+String(lon,4),String(radius)+" NM",String(brightness)+"%",String(rate)+" sec",modes[mode],units[unit],weatherAirport,radarStyle?"Geographic":"Retro",boardStyle?"Airport board":"Standard",nightDim?"ON":"OFF",flyover?"ON":"OFF",photosEnabled?"ON":"OFF","Clear settings"};for(int i=0;i<7;i++){int index=settingsPage*7+i,y=40+i*30;tft.fillRoundRect(8,y,464,27,3,CARD);textAt(names[index],17,y+6,2,TFT_WHITE,220);textAt(vals[index],250,y+9,1,ACCENT,200);textAt(">",456,y+6,2,MUTED);}btn(8,253,464,29,settingsPage?"PREVIOUS PAGE":"NEXT PAGE");nav();}

void drawOptions(){const char* titles[]={"","","RADAR RADIUS","BRIGHTNESS","REFRESH INTERVAL","DISPLAY MODE","UNITS"};header(titles[option]);if(option==3){textAt(String(brightness)+"%",175,75,4,TFT_WHITE);tft.fillRect(30,155,420,20,CARD);tft.fillRect(30,155,420*brightness/100,20,ACCENT);btn(30,200,190,55,"- 10%");btn(260,200,190,55,"+ 10%");}else{int n=option==2?5:option==6?3:4;for(int i=0;i<n;i++){String s;bool active=false;if(option==2){s=String(radii[i])+" nautical miles";active=radius==radii[i];}if(option==4){s=String(rates[i])+" seconds";active=rate==rates[i];}if(option==5){s=modes[i];active=mode==i;}if(option==6){s=units[i];active=unit==i;}btn(15,48+i*44,450,39,(active?"* ":"")+s,active?0x2450:CARD);}}back();}
void drawNetworks(){header("WIFI NETWORKS");for(int i=0;i<5&&netOffset+i<netCount;i++)btn(10,42+i*40,460,35,networks[netOffset+i]);if(!netCount)textAt("No networks found. Rescan or enter manually.",10,75,1,MUTED);btn(8,247,85,32,"PREV");btn(100,247,85,32,"NEXT");btn(192,247,85,32,"RESCAN");btn(284,247,85,32,"MANUAL");btn(376,247,96,32,"PHONE SETUP");back();}
String keysFor(int r){if(keyboardMode==0){const char* a[]={"1234567890","qwertyuiop","asdfghjkl","zxcvbnm"};return a[r];}if(keyboardMode==1){const char* a[]={"1234567890","QWERTYUIOP","ASDFGHJKL","ZXCVBNM"};return a[r];}const char* a[]={"!@#$%^&*()","-_=+[]{}|\\",";:'\",.<>?","/~`"};return a[r];}
void drawKeyboard(){header(editField==0?"ENTER NETWORK NAME":editField==1?"ENTER WIFI PASSWORD":editField==2?"ENTER LATITUDE":editField==3?"ENTER LONGITUDE":"AIRPORT ICAO OR AUTO");tft.fillRoundRect(10,42,460,38,4,CARD);String shown=edit;if(editField==1&&!showPassword){shown="";for(unsigned i=0;i<edit.length();i++)shown+="*";}if(shown.length()>33)shown=shown.substring(shown.length()-33);textAt(shown,18,53,2,TFT_WHITE,400);if(editField==1)btn(410,45,55,31,showPassword?"HIDE":"SHOW");if((editField==2||editField==3)){const char* k[]={"1","2","3","4","5","6","7","8","9","+/-","0","."};for(int i=0;i<12;i++)btn(15+(i%3)*152,86+(i/3)*39,146,35,k[i]);btn(298,245,85,35,"DELETE");btn(388,245,84,35,"OK",0x2450);btn(8,286,110,34,"CANCEL");textAt(notice,125,296,1,TFT_YELLOW,345);return;}for(int r=0;r<4;r++){String k=keysFor(r);int start=(480-k.length()*44)/2;for(unsigned j=0;j<k.length();j++)btn(start+j*44,88+r*39,41,35,String(k[j]));}btn(8,245,75,35,"SHIFT");btn(88,245,75,35,"SYMBOLS");btn(168,245,125,35,"SPACE");btn(298,245,85,35,"DELETE");btn(388,245,84,35,"OK",0x2450);btn(8,286,110,34,"CANCEL");textAt(notice,125,296,1,TFT_YELLOW,345);}
void drawPortal(){header("PHONE CONFIGURATION");textAt("Connect your phone to WiFi:",15,55,2,TFT_WHITE);textAt("FlightRadar-Setup",15,83,3,ACCENT);textAt("Password: "+apPass,15,120,2,TFT_WHITE);textAt("Open http://192.168.4.1",15,155,2,TFT_WHITE);textAt("Stay connected even if phone says no internet.",15,192,1,MUTED);textAt("Setup closes when you press Back or save.",15,210,1,MUTED);back();}
void draw(){if(page!=RADAR)releaseRadar();switch(page){case RADAR:drawRadar();break;case FLIGHTS:drawList();break;case DETAILS:drawDetails();break;case WEATHER:drawWeather();break;case SETTINGS:drawSettings();break;case OPTIONS:drawOptions();break;case NETWORKS:drawNetworks();break;case KEYBOARD:drawKeyboard();break;case PORTAL:drawPortal();break;case RESET_CONFIRM:header("FACTORY RESET");textAt("Erase saved WiFi and all settings?",20,80,2,TFT_WHITE);btn(20,165,210,60,"CANCEL");btn(250,165,210,60,"ERASE AND RESTART",TFT_RED);break;}}
bool parseCoord(String s,float limit,float &out){bool digit=false,dot=false;for(unsigned i=0;i<s.length();i++){char c=s[i];if(c>='0'&&c<='9')digit=true;else if(c=='.'&&!dot)dot=true;else if(c=='-'&&i==0){}else return false;}if(!digit)return false;char *end;float v=strtof(s.c_str(),&end);if(end==s.c_str()||*end||!isfinite(v)||fabs(v)>limit)return false;out=v;return true;}
void clearFlights(){generation++;lastGood=0;flightFailures=0;stationCount=placeCount=mapLineCount=0;wx.valid=false;zoneKnown=false;weatherWanted=geoWanted=zoneWanted=true;detailWanted=imagesWanted=true;for(auto &t:trails){t.n=0;t.hex="";}count=0;selected=-1;selectedHex="";follow=true;offset=0;online=false;currentFlightDetail=Detail();forceFetch=true;}
void openKeyboard(int field,String initial){editField=field;edit=initial;keyboardMode=0;showPassword=false;notice="";page=KEYBOARD;draw();}
bool tryNetwork(String name,String password){header("CONNECTING WIFI");textAt(name,15,80,2,TFT_WHITE);WiFi.mode(WIFI_STA);WiFi.disconnect(false,false);WiFi.begin(name.c_str(),password.c_str());unsigned long start=millis();while(WiFi.status()!=WL_CONNECTED&&millis()-start<15000){delay(250);}return WiFi.status()==WL_CONNECTED;}
void connectSaved(){if(ssid.length()&&tryNetwork(ssid,pass)){Serial.printf("WiFi connected: %s RSSI %d dBm\n",WiFi.localIP().toString().c_str(),WiFi.RSSI());configTime(0,0,"pool.ntp.org","time.nist.gov");page=mode==3?DETAILS:RADAR;forceFetch=true;draw();return;}scanNetworks();}
void scanNetworks(){if(portal)stopPortal();header("SCANNING WIFI");WiFi.mode(WIFI_STA);int n=WiFi.scanNetworks();netCount=0;for(int i=0;i<n&&netCount<40;i++){String s=WiFi.SSID(i);if(!s.length())continue;bool exists=false;for(int j=0;j<netCount;j++)if(networks[j]==s)exists=true;if(!exists)networks[netCount++]=s;}WiFi.scanDelete();netOffset=0;page=NETWORKS;draw();}
void stopPortal(){if(!portal)return;server.stop();WiFi.softAPdisconnect(true);WiFi.mode(WIFI_STA);portal=false;}
String htmlEscape(String s){s.replace("&","&amp;");s.replace("<","&lt;");s.replace(">","&gt;");s.replace("\"","&quot;");s.replace("'","&#39;");return s;}
void portalHome(){if(server.client().localIP()!=WiFi.softAPIP()){server.send(403,"text/plain","Use the setup WiFi network.");return;}String h="<!doctype html><html><meta name='viewport' content='width=device-width,initial-scale=1'><style>body{font:17px system-ui;max-width:480px;margin:24px auto;padding:12px;background:#101b29;color:white}input,select,button{box-sizing:border-box;width:100%;padding:12px;margin:6px 0 16px;font:inherit}button{background:#3185c6;color:white;border:0}</style><h2>FlightRadar setup</h2><form method='post' action='/save'><label>Wi-Fi name</label><input name='ssid' maxlength='32' required value='"+htmlEscape(ssid)+"'><label>Wi-Fi password (leave empty to retain saved password for same network)</label><input name='pass' type='password' maxlength='64'><label>Latitude</label><input name='lat' type='number' step='any' min='-90' max='90' required value='"+String(lat,6)+"'><label>Longitude</label><input name='lon' type='number' step='any' min='-180' max='180' required value='"+String(lon,6)+"'>";
h+="<label>Radar radius (NM)</label><select name='radius'>";for(int r:radii)h+="<option value='"+String(r)+"'"+(r==radius?" selected":"")+">"+String(r)+"</option>";h+="</select><label>Brightness (%)</label><input name='bright' type='number' min='10' max='100' required value='"+String(brightness)+"'><label>Refresh seconds</label><select name='rate'>";for(int r:rates)h+="<option value='"+String(r)+"'"+(r==rate?" selected":"")+">"+String(r)+"</option>";h+="</select><label>Display mode</label><select name='mode'>";for(int i=0;i<4;i++)h+="<option value='"+String(i)+"'"+(i==mode?" selected":"")+">"+modes[i]+"</option>";h+="</select><label>Units</label><select name='unit'>";for(int i=0;i<3;i++)h+="<option value='"+String(i)+"'"+(i==unit?" selected":"")+">"+units[i]+"</option>";h+="</select><button>Save and reconnect</button></form></html>";server.send(200,"text/html",h);}
bool portalPending=false;
void portalSave(){if(server.client().localIP()!=WiFi.softAPIP()){server.send(403,"text/plain","Use the setup WiFi network.");return;}float a,b;String s=server.arg("ssid"),p=server.arg("pass");int r=server.arg("radius").toInt(),v=server.arg("rate").toInt(),br=server.arg("bright").toInt(),m=server.arg("mode").toInt(),u=server.arg("unit").toInt();if(!s.length()||s.length()>32||p.length()>64||!parseCoord(server.arg("lat"),90,a)||!parseCoord(server.arg("lon"),180,b)||!validChoice(r,radii,5)||!validChoice(v,rates,4)||br<10||br>100||m<0||m>3||u<0||u>2){server.send(400,"text/plain","Invalid settings. Go back and try again.");return;}if(!p.length()&&s==ssid)p=pass;ssid=s;pass=p;lat=a;lon=b;radius=r;rate=v;brightness=br;mode=m;unit=u;saveSettings();server.send(200,"text/html","<h2>Saved. FlightRadar will reconnect now.</h2><p>If connection fails, use setup again.</p>");portalPending=true;}
void startPortal(){if(portal)return;WiFi.mode(WIFI_AP_STA);apPass=String(10000000UL+esp_random()%90000000UL);if(!WiFi.softAP("FlightRadar-Setup",apPass.c_str())){page=NETWORKS;draw();return;}server.begin();portal=true;page=PORTAL;draw();}
void choose(int i,bool nearest){if(i<0||i>=count)return;if(i==selected){follow=nearest;draw();return;}selected=i;selectedHex=planes[i].hex;follow=nearest;loadDetails();draw();}

void handleTouch(int x,int y){lastCycle=millis();if(page<=SETTINGS&&y>=283){page=(Page)constrain(x/96,0,4);if(page==DETAILS)imagesWanted=true;draw();return;}if(page==RADAR){if(y>=230&&y<256&&count&&((x>=266&&x<333)||x>=417)){choose((selected+count+(x<333?-1:1))%count,false);return;}if(x>=266&&x<372&&y>=256){choose(0,true);return;}if(x>265&&y>40){page=DETAILS;imagesWanted=true;draw();return;}if(x<265&&y>35){int best=-1,d=48*48;for(int i=0;i<count;i++){if(!visiblePlane(i))continue;int px,py,lx,ly,w;position(i,px,py);radarLabel(i,lx,ly,w);int q=sq(x-px)+sq(y-py);int dx=max(0,max(lx-x,x-(lx+w))),dy=max(0,max(ly-y,y-(ly+8)));if((!radarStyle||i==selected||i<4)&&dx<=15&&dy<=16)q=min(q,dx*dx+dy*dy+25);if(q<d){d=q;best=i;}}if(best>=0)choose(best,false);}return;}
if(page==FLIGHTS){if(y>=251){if(x<155)offset=max(0,offset-5);else if(x>325&&offset+5<count)offset+=5;draw();}else if(y>=56&&y<251){int i=offset+constrain((y-57)/38,0,4);if(i<count){page=DETAILS;choose(i,false);}}return;}
if(page==SETTINGS&&y>=38&&y<283){if(y>=250){settingsPage=1-settingsPage;draw();return;}option=settingsPage*7+constrain((y-40)/30,0,6);if(option==0){scanNetworks();return;}if(option==1){openKeyboard(2,String(lat,6));return;}if(option==7){openKeyboard(4,weatherAirport);return;}if(option==8){radarStyle=1-radarStyle;releaseRadar();geoWanted=true;}if(option==9)boardStyle=1-boardStyle;if(option==10){nightDim=!nightDim;applyBrightness();}if(option==11)flyover=!flyover;if(option==12){photosEnabled=!photosEnabled;imagesWanted=true;}if(option>=8&&option<=12){saveSettings();draw();return;}page=option==13?RESET_CONFIRM:OPTIONS;draw();return;}
if(page==OPTIONS){if(y>=283){page=SETTINGS;draw();return;}if(option==3&&y>=192&&y<266){brightness=constrain(brightness+(x<240?-10:10),10,100);applyBrightness();saveSettings();draw();return;}int i=max(0,(y-46)/44),n=option==2?5:option==6?3:4;if(y>=44&&y<48+n*44&&i<n&&option!=3){if(option==2){radius=radii[i];clearFlights();}if(option==4){rate=rates[i];}if(option==5)mode=i;if(option==6)unit=i;saveSettings();draw();}return;}
if(page==NETWORKS){if(y>=283){page=SETTINGS;draw();return;}if(y>=247){if(x<94){netOffset=max(0,netOffset-5);draw();}else if(x<190){if(netOffset+5<netCount)netOffset+=5;draw();}else if(x<282)scanNetworks();else if(x<375)openKeyboard(0,"");else startPortal();return;}if(y>=39&&y<242){int i=netOffset+constrain((y-39)/40,0,4);if(i<netCount){editSSID=networks[i];openKeyboard(1,"");}}return;}
if(page==PORTAL){if(y>=283){stopPortal();page=SETTINGS;draw();}return;}
if(page==RESET_CONFIRM){if(y>=151&&y<240){if(x<240){page=SETTINGS;draw();}else{prefs.clear();WiFi.disconnect(true,true);delay(300);ESP.restart();}}return;}
if(page==KEYBOARD){if(y>=286){page=editField>=2?SETTINGS:NETWORKS;draw();return;}if(y>=42&&y<80&&x>=410&&editField==1){showPassword=!showPassword;draw();return;}if((editField==2||editField==3)&&y>=86&&y<242){int col=constrain((x-15)/152,0,2),row=(y-86)/39;if(x>=0&&x<=479&&col>=0&&col<3&&row>=0&&row<4){int i=row*3+col;if(i==9){if(edit.startsWith("-"))edit.remove(0,1);else edit="-"+edit;}else if(i==11){if(edit.indexOf('.')<0&&edit.length()<20)edit+='.';}else if(edit.length()<20){edit+=(char)(i==10?'0':'1'+i);}notice="";}draw();return;}if((editField==2||editField==3)&&y>=245&&y<280&&x<298)return;if((editField<2||editField==4)&&y>=88&&y<244){int r=(y-88)/39;String k=keysFor(r);int start=(480-k.length()*44)/2;if(x>=start-8&&x<start+(int)k.length()*44+8){int j=constrain((x-start)/44,0,(int)k.length()-1);if(j<(int)k.length()&&edit.length()<(editField==0?32:editField==1?64:20))edit+=k[j];}draw();return;}if(y>=245&&y<280){if(x<85)keyboardMode=keyboardMode==1?0:1;else if(x<165)keyboardMode=keyboardMode==2?0:2;else if(x<295){if(edit.length()<(editField==0?32:editField==1?64:20))edit+=' ';}else if(x<388){if(edit.length())edit.remove(edit.length()-1);}else{if(editField==0){if(!edit.length()){notice="Enter a network name";draw();return;}editSSID=edit;openKeyboard(1,"");return;}if(editField==1){String candidate=edit;if(tryNetwork(editSSID,candidate)){ssid=editSSID;pass=candidate;configTime(0,0,"pool.ntp.org","time.nist.gov");saveSettings();clearFlights();page=RADAR;draw();return;}notice="Connection failed. Retry or cancel.";edit=candidate;draw();return;}if(editField==4){edit.trim();edit.toUpperCase();bool ok=edit.length()==4;for(unsigned i=0;i<edit.length();i++)if(!isalnum((unsigned char)edit[i]))ok=false;if(!ok){notice="Enter 4-letter ICAO or AUTO";draw();return;}weatherAirport=edit;wx.valid=false;generation++;weatherWanted=true;forceFetch=true;saveSettings();page=SETTINGS;draw();return;}float v;if(!parseCoord(edit,editField==2?90:180,v)){notice="Invalid coordinate. Check range.";draw();return;}if(editField==2){pendingLat=v;openKeyboard(3,String(lon,6));return;}lat=pendingLat;lon=v;saveSettings();clearFlights();page=SETTINGS;draw();return;}draw();}return;}}
void setup(){Serial.begin(115200);pinMode(BL,OUTPUT);digitalWrite(BL,HIGH);tft.init();tft.setRotation(1);tft.setTouch(calData);prefs.begin("flightradar",false);loadSettings();pwm=ledcAttach(BL,5000,8);applyBrightness();WiFi.persistent(false);WiFi.setAutoReconnect(true);WiFi.setSleep(false);configTime(0,0,"pool.ntp.org","time.nist.gov");workQ=xQueueCreate(1,sizeof(Job*));resultQ=xQueueCreate(1,sizeof(Result*));if(workQ&&resultQ)xTaskCreatePinnedToCore(networkWorker,"flightNet",12288,nullptr,1,&workerHandle,0);if(!workerHandle)Serial.println("ERROR: network worker allocation failed");server.on("/",HTTP_GET,portalHome);server.on("/save",HTTP_POST,portalSave);server.onNotFound(portalHome);connectSaved();lastCycle=millis();lastConnect=millis();}
void loop(){
 if(portal){server.handleClient();if(portalPending){portalPending=false;delay(400);stopPortal();applyBrightness();clearFlights();connectSaved();}}
 // Accept lighter finger pressure and debounce release to prevent repeated taps.
 static unsigned long lastTouchSeen=0;
 uint16_t x=0,y=0;bool pressed=tft.getTouch(&x,&y,350);
 if(pressed){lastTouchSeen=millis();if(!held){held=true;handleTouch(constrain((int)x,0,479),constrain((int)y,0,319));lastTouchSeen=millis();}}
 else if(held&&millis()-lastTouchSeen>=75){held=false;}
 if(!held)pollNetwork();
 if(!held&&!portal&&page<=WEATHER){if(WiFi.status()!=WL_CONNECTED){online=false;if(!wifiLostAt){wifiLostAt=millis();Serial.printf("WiFi lost: status %d\n",(int)WiFi.status());WiFi.reconnect();}if(!networkBusy&&millis()-wifiLostAt>=30000&&millis()-lastConnect>30000){lastConnect=millis();wifiLostAt=0;connectSaved();}}else{if(wifiLostAt){wifiLostAt=0;forceFetch=true;Serial.printf("WiFi restored: %s RSSI %d\n",WiFi.localIP().toString().c_str(),WiFi.RSSI());}scheduleNetwork();}
 if(mode==1&&millis()-lastCycle>=15000){page=(Page)((page+1)%4);lastCycle=millis();if(page==DETAILS)imagesWanted=true;draw();}else if((mode==2||mode==3)&&millis()-lastCycle>=30000){Page target=mode==2?RADAR:DETAILS;if(page!=target){page=target;if(mode==3&&count)choose(0,true);else draw();}lastCycle=millis();}}
 if(page==RADAR&&!radarStyle&&!held&&!portal&&!networkBusy&&millis()-lastFrame>=120){sweep=(sweep+6)%360;drawRadarFrame();}
 if(!held&&page<=WEATHER&&millis()-lastClockDraw>=1000){lastClockDraw=millis();drawStatusHeader();applyBrightness();}
 delay(25);
}
