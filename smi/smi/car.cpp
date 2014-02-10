#include <stdlib.h>
#include <stdio.h>
#include "car.h"
#include <time.h>
car *carlist[1000];
//freeCar链表为单向
carlink *freeCar;
carlink *freeCarTail;
//runCar维护双向
carlink *runCar;
carlink *runCarTail;

int carNum = 0;

void initializeCars(){
	srand(time(NULL));

	runCar  = (carlink*)malloc(sizeof(carlink));
	runCar->here = newcar();
	runCar->next = NULL;
	runCar->prior = NULL;
	runCarTail = runCar;
	/*
	for(int i=0; i<1000; i++){
		carlist[i] = newcar();
		carlist[i]->speed = getNewspeed();
	}
	*/
}
void addnewNode(carlink *Tail, carlink * newnode){
	Tail->next = newnode;
	newnode ->prior = Tail;
}
carlink *findbackcar(carlink *linkcc , int lane){
	carlink *p = linkcc->next;
	while(p!=NULL){
		int len=0;
		if(p->here->lane == L1L2
			|| p->here->lane == lane)  //如果后面一辆车是在变道  或者 后面一辆车在同一道上，判断车距
		{
			len = linkcc->here->location - p->here->location;
			if(len < p->here->safe_length)
			{
				return p;
			}
			else //车距安全
				return NULL;

		}
		if(len > 200)
			break;
		p = p->next;
	}
	return NULL;
}


carlink * findfrontcar(carlink *linkcc , int lane){  //
	carlink *p = linkcc->prior;

	while(p!=NULL){
		if(p->here->lane == L1L2
			|| p->here->lane == lane)  //如果前一辆车是在超车  或者 前一辆车在同一道上，判断车距
		{
			if(p->here->location < linkcc->here->location+linkcc->here->safe_length)
			{
				//车距过小时
				if(p->here->nowspeed+1 < linkcc->here->speed)
					return p;
				else
					return NULL;
			}
			else //车距安全
				return NULL;
		}
		if(  p->here->location  - linkcc->here->location  > 200)
			break;
		p = p->prior;
	}
	return NULL;

}

carlink *getNewCar(){
	int x = rand()%100;
	if(x>COMING){
		carlink *x  = (carlink*)malloc(sizeof(carlink));
		x->here = newcar();
		x->next = NULL;
		x->prior = NULL;
		return x;
	}
	return NULL;
}


float getNewspeed(){
	
	int x = rand()%10;
	if(x <1)
		return 60;
	if(x < 3)
		return 80;
	if(x < 7)
		return 100;
	if(x < 10)
		return 120;
}

int getSafel(int sp){
	return sp*18;
}
car* newcar(){
	car *p = (car*)malloc(sizeof(car));
	p->num = carNum++;
	p->state = STOP;
	p->speed = getNewspeed()/36*2;     //km/h -> m/0.2s
	p->nowspeed = p->speed;
	p->safe_length = getSafel(p->nowspeed);
	p->location = 0;
	p->lane = L1;
	return p;
}


void swaploc(carlink *b,carlink* c){
	carlink *a,*d,*temp2;
	
	if(b->next != NULL){
		if(c->prior !=0)
		{
			a=b->next;
			d=c->prior;
			a->prior = c;
			c->next = a;
			b->prior = d;
			d->next = b;
			b->next = c;
			c->prior = b;

		}
		else
		{
			a=b->next;
			d=c->prior;
			a->prior = c;
			c->next = a;
			b->prior = d;

			b->next = c;
			c->prior = b;
		}
	}
	else
	{
		if(c->prior !=0)
		{
			a=b->next;
			d=c->prior;

			c->next = a;
			b->prior = d;
			d->next = b;
			b->next = c;
			c->prior = b;
		}
		else
		{
			a=b->next;
			d=c->prior;
			//a->prior = c;
			c->next = a;
			b->prior = d;
			//d->next = b;
			b->next = c;
			c->prior = b;
		}
	}

}