#ifndef _CAR_
#define _CAR_
#define STOP 0
#define FREE 1
#define FOLLOW 2
#define L1 0
#define L2 1
#define L1L2 2
#define CARLENTH 5
#define COMING 10

typedef struct car{
	int num;
	float speed;
	float nowspeed;
	int state;
	int safe_length;
	float location;
	int lane;
};

typedef struct carlink{
	carlink *prior;
	car *here;
	carlink *next;
};

extern car *carlist[1000];
extern carlink *freeCar;
extern carlink *freeCarTail;
extern carlink *runCar;
extern carlink *runCarTail;
int getSafel(int sp);
void initializeCars();
carlink * findfrontcar(carlink *linkcc , int lane);
carlink *getNewCar();
void addnewNode(carlink *Tail, carlink * newnode);
void swaploc(carlink *b,carlink* c);
car* newcar();
carlink *findbackcar(carlink *linkcc , int lane);
#endif