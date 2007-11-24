/*
 * See Licensing and Copyright notice in naev.h
 */


#include "board.h"

#include "naev.h"
#include "pilot.h"
#include "player.h"
#include "toolkit.h"
#include "space.h"
#include "rng.h"
#include "economy.h"


#define BOARDING_WIDTH  300
#define BOARDING_HEIGHT 200


/*
 * externs
 */
extern unsigned int player_target;


static unsigned int board_credits = 0; /* money on the ship */


/*
 * prototypes
 */
static void player_unboard( char* str );


/*
 * attempt to board the player's target
 */
void player_board (void)
{  
	Pilot *p;
	unsigned int wid;
	char str[128];
	char cred[10];

	if (player_target==PLAYER_ID) {
		player_message("You need a target to board first!");
		return;
	}

	p = pilot_get(player_target);

	if (!pilot_isDisabled(p)) {
		player_message("You cannot board a ship that isn't disabled!");
		return;
	} if (vect_dist(&player->solid->pos,&p->solid->pos) >
			p->ship->gfx_space->sw * PILOT_SIZE_APROX) {
		player_message("You are too far away to board your target");
		return;
	} if ((pow2(VX(player->solid->vel)-VX(p->solid->vel)) +
				pow2(VY(player->solid->vel)-VY(p->solid->vel))) >
			(double)pow2(MAX_HYPERSPACE_VEL)) {
		player_message("You are going to fast to board the ship");
		return;
	}

	player_message("Boarding ship %s", p->name);

	/* calculate credits based on ship price */
	board_credits = RNG(20*p->ship->price, 50*p->ship->price)/1000;

	/*
	 * create the boarding window
	 */
	wid = window_create( "Boarding", -1, -1, BOARDING_WIDTH, BOARDING_HEIGHT );

	window_addText( wid, 20, -30, 120, 60,
			0, "txtCargo", &gl_smallFont, &cDConsole,
			"Credits:\n"
			"Cargo:\n"
			);
	credits2str( cred, board_credits );
	snprintf( str, 128,
			"%s\n"
			"%s\n"
			, cred, "none" );
	window_addText( wid, 80, -30, 120, 60,
			0, "txtData", &gl_smallFont, &cBlack, str );

	window_addButton( wid, 20, 20, 50, 30, "btnStealCredits", "Credits", player_unboard );

	window_addButton( wid, -20, 20, 50, 30, "btnBoardingClose", "Close", player_unboard );
}
static void player_unboard( char* str )
{  
	if (strcmp(str,"btnBoardingClose")==0)                                 
		window_destroy( window_get("Boarding") );
}

