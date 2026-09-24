#include <syslog.h>

int main() {
    openlog("MaSalle", LOG_PID, LOG_USER);

    syslog(LOG_INFO, "Demarrage du programme");
    syslog(LOG_INFO, "Programme initialise");
    syslog(LOG_INFO, "Programme pret");

    closelog();

    return 0;
}