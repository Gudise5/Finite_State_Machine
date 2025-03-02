/*
 * airplane.c
 *
 *  Created on: 03-Sep-2022
 *      Author: Gudise Eshwar
 */

#include<stdio.h>
#include<stdlib.h>

typedef enum State
{
	GR_DOWN, //here calling the different state
	CHK_BFR_TKOFF,
	RISING_GR,
	GEAR_UP,
	CHK_BFR_LAND,
	LOWERING_GR
}State_type;

typedef enum Switch //declares an alternative name as switch
{
	Open, //here calling the different switch
	Close
}Switch_Status;

typedef enum Pilot_Lever //declares an alternative name as pilot lever
{
	Raising, //here calling the different pilot lever
	Falling,
	UP,
	DOWN
}Pilot_Lever_Status;

void Init_State_Machine();
void Gear_Down();
void Checking_Before_Takeoff();
void Raising_Gear();
void Gear_Up();
void Checking_Before_Landing();
void Lowering_Gear();

static volatile Switch_Status Squat_Switch;
static volatile Switch_Status limit_switch;
static volatile Pilot_Lever_Status pilot_lever;
State_type current_state;                       //current state is the state type


typedef struct //declaration of structure using typedef
{
	char* current_state_indication;
	char* light_status;
	char* direction_valve_status;
}State_Table;

//calling the  state_ Table

static State_Table State_Machine[6]={

{"GearDown","Green","Down"},

{"Checking Before Take Off","Green","Down"},

{"Rising Gear","Red","Up"},

{"Gear Up","Off","Null"},

{"Checking Before Landing","Red","Down"},

{"Lowerig Gear","Green","Down"}

};

//using state table the condition controlling the retraction and extension of airoplane's landig gear

static void(*state_table[])(void) = {Gear_Down,Checking_Before_Takeoff,Raising_Gear,Gear_Up,Checking_Before_Landing,Lowering_Gear};

int main()
{
Init_State_Machine(); //declare initial state machine

while(1)
	{
		state_table[current_state](); // current state is the declare in the state table
	}
}

void Init_State_Machine() //initial state machine condition
{
	current_state = GR_DOWN; //current state is gear down
	printf("\n State Machine is initialized & Program is in Gear_Down_state & Light_Status :%s\n",State_Machine[current_state].light_status);

}

void Gear_Down(void)
{
	current_state = GR_DOWN; //current state is gear down
	printf("\nEnter the pilot's Lever Position, Squat Switch Status ");
	fflush(stdout);
	scanf("%d %d",&pilot_lever, &Squat_Switch); //reads to console pilot lever and squat switch

	if(pilot_lever == Raising && Squat_Switch == Open)
		{
			current_state = CHK_BFR_TKOFF;                                                               //current state is check before takeoff
			printf("\n Current State :%s\n",State_Machine[current_state].current_state_indication);      //print current state
			printf("\nLight State :%s\n",State_Machine[current_state].light_status);                     //print light status
		}
	else
		{
		Gear_Down();
		}
}

void Checking_Before_Takeoff() //checking before takeoff condition
{
	current_state = CHK_BFR_TKOFF; //current state is checking before takeoff
	printf("\n Enter the pilot's lever Position,Squat Switch Status :");
	fflush(stdout);
	scanf("%d %d",&pilot_lever,&Squat_Switch);
    printf("\n Wait for 2 sec\n");
	if(pilot_lever == Falling || Squat_Switch == Close)
		{
			Gear_Down();
		}
	else if(pilot_lever == UP && Squat_Switch == Open)
		{
			current_state = RISING_GR; //current state is rising gear
			printf("\n Current State :%s\n",State_Machine[current_state].current_state_indication); //print current state
			printf("\n Light Status :%s\n",State_Machine[current_state].light_status); //print light status
			printf("Direction valve Staus :%s\n",State_Machine[current_state].direction_valve_status); //print direction value
		}
	else
	{
		Checking_Before_Takeoff();
	}
}

void Raising_Gear() // raising gear operation
{
	current_state = RISING_GR; // current state is raising gear
	printf("\n Enter the Limit Switch Status,Pilots Lever Status:");
	fflush(stdout);
	scanf("%d  %d",&limit_switch,&pilot_lever); //reads to console limit switch ,pilot lever
	printf("\n Wait for 1 sec\n");
	if(pilot_lever == Falling && limit_switch == Open)
	{
		current_state=CHK_BFR_LAND;                                                              //current state is checking before landing
    	printf("\n Current State :%s\n",State_Machine[current_state].current_state_indication); //print current state
    	printf("\n Light State :%s\n",State_Machine[current_state].light_status);                //print light status
	}
	else if(limit_switch == Close && pilot_lever == UP)
	{
		current_state = GEAR_UP; //current state is gear up
		printf("\n Current State :%s\n",State_Machine[current_state].current_state_indication); // print current state
		printf("\n Light State :%s\n",State_Machine[current_state].light_status); //print light status
	}
	else
	{
	Raising_Gear();
	}
}

void Gear_Up()
{
	current_state = GEAR_UP; //current state is gear up
	printf("\n Enter the Pilot's lever Position :");
	fflush(stdout);
	scanf("%d",&pilot_lever); //reads to console pilot lever
	if(pilot_lever == Falling) //if pilot lever is falling
	{
		current_state=CHK_BFR_LAND; //current state is checking before landing
		printf("\n Current State :%s\n",State_Machine[current_state].current_state_indication); //print current state
		printf("\nLight Status :%s\n",State_Machine[current_state].light_status); //print light status
	}
	else
	{
		Gear_Up();
	}
}

void Checking_Before_Landing()
{
	current_state = CHK_BFR_LAND; // current state is checking before landing
	printf("\n Enter the Pilot's Lever Position:");
	fflush(stdout);
	scanf("%d",&pilot_lever);
	if(pilot_lever == DOWN )
	{
		current_state = LOWERING_GR; //current state is lowering gear
    	printf("\n Current State:%s\n",State_Machine[current_state].current_state_indication); //print current state
    	printf("\nLight State: %s\n",State_Machine[current_state].light_status); //print light status
   		printf("Direction Valve Status:%s\n\n",State_Machine[current_state].direction_valve_status);
	}
	if(pilot_lever == (UP||Raising||Falling)) //if pilot lever is up,raising or falling
	{
		Checking_Before_Landing();
	}
}

void Lowering_Gear()
{
	current_state = LOWERING_GR; //current state is lowering gear
	printf("\n Enter the limit switch Squat Status.pilot's lever Position :");
	fflush(stdout);
	scanf("%d %d",&pilot_lever,&limit_switch);
	if(limit_switch == Close && pilot_lever == Raising) //if limit switch is close then pilot lever is raising
	{
		current_state = CHK_BFR_TKOFF;
		printf("\n Current State :%s\n",State_Machine[current_state].current_state_indication); //print current state
		printf("\n Light Status :%s\n",State_Machine[current_state].light_status); //print light status
	}
	if(limit_switch == Open && pilot_lever == DOWN)
	{
		current_state = GR_DOWN; //current state is gear down
		printf("\n Current State :%s\n",State_Machine[current_state].current_state_indication); //print current state
		printf("\n Light Status :%s\n",State_Machine[current_state].light_status); //print light_status
	}
	else
	{
		Lowering_Gear();
	}
}

















