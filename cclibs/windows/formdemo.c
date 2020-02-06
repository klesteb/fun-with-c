
#include "windows.h"

#define NUMBER_FIELDS 12

int main(void) {

	int i;
	Windows *mainwin;
	struct form_struct form[NUMBER_FIELDS + 1] = {
		1,  1,  "First name:",     NULL, 10,  EDT_ANYTHING,
		2,  1,  "Last name :",     NULL, 15,  EDT_ANYTHING,
		2, 40,  "Middle init:",    NULL, 1,   EDT_UPPER_CASE,
		4,  1,  "Age:",            NULL, 3,   EDT_NUMBER,
		4, 12,  "Sex:",            NULL, 1,   EDT_UPPER_CASE,
		4, 25,  "Date of birth:",  NULL, 10,  EDT_DATE,
		7,  1,  "Street address:", NULL, 30,  EDT_ANYTHING,
		8,  1,  "City:",           NULL, 20,  EDT_ANYTHING,
		9,  1,  "State:",          NULL, 3,   EDT_ANYTHING,
		9, 12,  "Zip:",            NULL, 11,  EDT_ANYTHING,
		11, 1,  "Current title:",  NULL, 40,  EDT_ANYTHING,
		13, 1,  "Comments:",       NULL, 160, EDT_ANYTHING,
		0
		};

	win_init();

	mainwin = win_open("Form", 20, 78, 1, 1, FALSE);

	for (i = 0; i < NUMBER_FIELDS; i++) {

		form[i].string = malloc(form[i].length + 1);
		form[i].string[0] = '\0';

	}

	i = win_forms(mainwin, form);

	win_close(mainwin);

	mainwin = win_open("Results", 20, 78, 1, 1, FALSE);

	for (i = 0; i < NUMBER_FIELDS; i++) {

		win_output(mainwin, i, 1, form[i].string);

	}

	i = win_getch(mainwin);

	win_close(mainwin);

	for (i = 0; i < NUMBER_FIELDS; i++) {

		free(form[i].string);

	}

	win_quit();

}

