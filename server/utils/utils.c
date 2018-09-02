#include "utils/utils.h"
#include "core/log/log.h"

void banner()
{
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    char displayed_time[64];
    strftime(displayed_time, sizeof(displayed_time), "%c", tm);
	
	log_trace("Starting at %s", displayed_time);
	log_info("");
	log_info("");
	log_info("  _________________  ________________________      __  _____ __________");
	log_info(" /   _____/\\_____  \\ \\_   _____/\\__    ___/  \\    /  \\/  _  \\______   \\");
	log_info(" \\_____  \\  /   |   \\ |    __)    |    |  \\   \\/\\/   /  /_\\  \\|       _/");
	log_info(" /        \\/    |    \\|     \\     |    |   \\        /    |    \\    |   \\");
	log_info("/_______  /\\_______  /\\___  /     |____|    \\__/\\  /\\____|__  /____|_  /");
	log_info("        \\/         \\/     \\/                     \\/         \\/       \\/ ");
	log_info("              Ready to prove that robot can't kill humans");
	log_info("                              version 0.1");
	log_info("");
	log_info("");
}