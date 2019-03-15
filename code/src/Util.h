
class Util {

public:
    void init_wifi();

    void init_time();

    void init_mqtt();

    void now_time();

    bool wake_on_lan();

    bool arping();

    void print(const char *msg);

    void println(const char *msg);

//  setter  and getter



private:
    int  wake_port              = 9;
    char wake_mac[18]           = "4C:ED:FB:BD:DC:E6";

    char mobile_ip[16]          = "192.168.3.52";
    char computer_ip[16]        = "192.168.3.49";

    char wifi_ssid[20]          = "Adesk";
    char wifi_password[20]      = "tryourbest";

    char notify_host[12]        = "sc.ftqq.com";
    char notify_url[61]         = "/SCU44583Te387109551e70dc28790757bf3796aa05c643c7f5ac0c.send";

    char mqtt_host[50]          = "mqtt.wuxs.vip";
    int  mqtt_port              = 1883;
    char mqtt_username[50]      = "";
    char mqtt_password[50]      = "";

    int  http_port              = 80;

    bool make_magic_packet();

    void write_to_log();


};



