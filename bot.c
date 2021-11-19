#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include <orca/discord.h>
#include <orca/github.h>
#include "color.h"

void ready(struct discord *client, const struct discord_user *bot) {

	log_info("Bot was connected to discord as %s#%s!", bot->username, bot->discriminator);

}
void HELP(struct discord *client, const struct discord_user *bot, const struct discord_message *msg) {
	
	if(msg -> author -> bot) return;

	struct discord_embed embed = {
	.title = "HELP",
	.description = "Commands: \n>help - sends help\n>test - test\n>lspci - displays the pcie device in the TermBot server.\n>cpu - displays the CPU conponents in the TermBot server.\n>mem - displays the ammount of max and free memory the TermBot server has.\n>tree - lists an up to date TermBot directory list.",
	.color = 3447003
	};

	struct discord_create_message_params par = { .embed = &embed};
	discord_create_message(client, msg -> channel_id, &par, NULL);
	log_info("%s:: Help was used", BGREEN);
	printf("%s", RESET);


}
void test(struct discord *client, const struct discord_user *bot, const struct discord_message *msg) {

	if(msg -> author -> bot) return;
        struct discord_embed embed = {
        .title = "TEST",
        .description = "Testing, 1 2 3",
	.color = 3447003	
        };

	struct discord_create_message_params par = { .embed = &embed};
	discord_create_message(client, msg -> channel_id, &par, NULL);
	log_info("%s:: %s: Test was used", BGREEN, bot->username);
	printf("%s", RESET);

}

void lspci(struct discord *client, const struct discord_user *bot, const struct discord_message *msg) {

	if(msg -> author -> bot) return;

	FILE *NF;
	char line1[500];

	NF = popen("/bin/lspci | grep VGA", "r");

	if(!(NF)) {
	
		log_info("%s:: Failed to run lspci", BRED);	
	
	}

	while(fgets(line1, sizeof line1, NF) != NULL) {
		
		struct discord_embed embed = {
                .title = "lspci",
                .description = line1
                };

		struct discord_create_message_params par = { .embed = &embed};
		discord_create_message(client, msg -> channel_id, &par, NULL);
		log_info("%s:: lspci was used!", BGREEN);
		printf("%s", RESET);
	
	}
	pclose(NF);
}

void TREE(struct discord *client, const struct discord_user *bot, const struct discord_message *msg) {

	if(msg -> author -> bot) return;

	FILE *TRE;
	char line[500];

	TRE = popen("/bin/tree .", "r");

	if(!(TRE)) {
	
		log_info("%s:: Failed to run Tree", BRED);

	}

	while(fgets(line, sizeof line, TRE) != NULL) {
	
		struct discord_create_message_params par = { .content = line};
		discord_create_message(client, msg -> channel_id, &par, NULL);
		log_info("%s:: Tree was used!", BGREEN);
		printf("%s", RESET);
		
	
	}

}

void CPU(struct discord *client, const struct discord_user *bot, const struct discord_message *msg) {

	if(msg -> author -> bot) return;

	FILE *fp1;
	int count = 0;
	int LN = 4;
	
	fp1 = fopen("/proc/cpuinfo", "r");

	if(!(fp1)) {
	
		log_info("%s:: Failed to run CPU", BRED);
	
	}

	char line2[256];

	while(fgets(line2, sizeof line2, fp1) != NULL) {
	
		if(count == LN) {
			struct discord_embed embed = {
                        .title = "CPU",
                        .description = line2
                        };
	
			struct discord_create_message_params par = { .embed = &embed};
                	discord_create_message(client, msg -> channel_id, &par, NULL);
                	log_info("%s:: CPU was used!", BGREEN);
                	printf("%s", RESET);

			if(count == 4) {
				break;
			}
		} else {
			count++;
		}
	
	}
	fclose(fp1);

}

void MEM(struct discord *client, const struct discord_user *bot, const struct discord_message *msg) {
	
	if(msg -> author -> bot) return;

	FILE *fp4;
	int LN3 = 0;
	int LN4 = 1;
	int count2 = 0;

	fp4 = fopen("/proc/meminfo", "r");

	if(!(fp4)) {
	
		log_info("%s:: Failed to run Meminfo", BRED);

	}
	char line3[35];

	while(fgets(line3, sizeof line3, fp4) != NULL) {
	
		if(count2 == LN3) {
			struct discord_embed embed = {
    			.title = "Memory",
    			.description = line3
			};
			struct discord_create_message_params par = { .embed = &embed};
			discord_create_message(client, msg -> channel_id, &par, NULL);
			log_info("%s:: Meminfo was used!", BGREEN);
			printf("%s", RESET);
		if(count2 == 0) {
			break;
		}
		
		}

	}
}

int main(void) {

	char *token = "YOUR TOKEN";

	struct discord *client = discord_init(token);

	discord_set_on_ready(client, &ready);
	
	discord_set_on_command(client, ">test", &test);
	discord_set_on_command(client, ">lspci", &lspci);
	discord_set_on_command(client, ">cpu", &CPU);
	discord_set_on_command(client, ">mem", &MEM);
	discord_set_on_command(client, ">tree", &TREE);
	discord_set_on_command(client, ">help", &HELP);

	discord_set_presence(
    		client, &(struct discord_presence_status){
              		.activities =
                	(struct discord_activity *[]){
                  	&(struct discord_activity){ .name = " >help",
                                          	    .type = DISCORD_ACTIVITY_GAME,
                                                    .details = " >help" },
                  NULL
                }});

	discord_run(client);
}
