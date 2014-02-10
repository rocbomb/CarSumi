#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "car.h"
#define WAY_L 10000


bool way[WAY_L][2];


int CarChangeTimes = 0;
int Carinroad = 0;
int Carisover = 0;
int Carstart = 0;
double Carlost = 0;
void runthiscar(carlink *chere){
	car *qq = chere->here;

	int anything = 0;
	int where = qq->lane;
	carlink *p;



	if(where == L1){
		//���г��� �鿴ǰ������
		carlink* frontcarL1 = findfrontcar(chere, qq->lane);

		if(frontcarL1 == NULL){     //ǰ��û�� ����ȫ
			qq->nowspeed = qq->speed;
			qq->location += qq->nowspeed;
		}
		else
		{
			if(frontcarL1->here->nowspeed+0.1 < qq->speed ){
				//�Լ��ȽϿ�
				carlink* frontcarL2 = findfrontcar(chere, L2);
				if(frontcarL2 == NULL){  //������ǰ��û������
									//�ڿ����г��������ǲ����г�
					carlink* backcarL3 = findbackcar(chere, L2);
					if(backcarL3 == NULL){
						qq->state = 5;  //������ģʽ
						qq->lane = L1L2;
						qq->nowspeed = qq->speed; //����
						CarChangeTimes++;
					}
					else
						qq->nowspeed = frontcarL1->here->nowspeed;
				}
				else{ //������Ҳ�г� T_T
					//����ǰ�泵������
					qq->nowspeed = frontcarL1->here->nowspeed;
				
				}
				qq->location += qq->nowspeed;
			}
			else{     //ǰ��ĳ��ȽϿ� ���ܳ���
				qq->nowspeed = qq->speed;
				qq->location += qq->nowspeed;
			}


		}

	}
	else if(where == L2){
				//�ڳ����� �鿴ǰ���г����ͳ���������
		carlink* frontcarL1 = findfrontcar(chere, L1);
		carlink* frontcarL2 = findfrontcar(chere, L2);
		if(frontcarL1 == NULL){     //ǰ���г���û�� ��ȫ
			
			if(frontcarL2 == NULL){  //������ǰ��û������
				//�ڿ����г��������ǲ����г�  ������
				qq->state = -5;  //������ģʽ
				qq->lane = L1L2;
				qq->nowspeed = qq->speed; //����
				CarChangeTimes++;
			}else
			{
				//������Ҳ�г�����
				qq->nowspeed = frontcarL2->here->nowspeed;
			}

			qq->location += qq->nowspeed;
		}
		else
		{
			if(frontcarL2 == NULL){  //������ǰ��û������
				qq->nowspeed = qq->speed; //����
			}
			else{ //������Ҳ�г�  T_T
				//����ǰ�泵������
				qq->nowspeed = frontcarL2->here->nowspeed;
			}
			qq->location += qq->nowspeed;
		}
	}
	else //L1L2ģʽ  ���
	{
		//carlink* frontcarL1 = findfrontcar(chere, L1);
		//carlink* frontcarL2 = findfrontcar(chere, L2);
		//������ģʽ�����٣��Ҳ���ǰ�泵��
		if(qq->state < 0){ //L2 -> L1
			qq->state++;
			if(qq->state == 0)
				qq->lane = L1;
		}
		else //L1 -> L2
		{
			qq->state--;
			if(qq->state == 0)
				qq->lane = L2;
		}
		qq->location += qq->nowspeed;
	}
	if(qq->nowspeed != qq->speed)
		Carlost += qq->speed - qq->nowspeed;
}

int main(){
	FILE *output;
	output = fopen("11061046_token.txt","w");
	initializeCars();
	
	carlink * carhere;
	car *qq;
	//�Ȱѵ�һ��������ȥ
	runthiscar(runCar);
	carlink * chere;
	int nx=15;
	while(nx--){
		Carinroad = 0;
		Carisover = 0;
		CarChangeTimes = 0;
		Carstart = 0;
		Carlost = 0;
	for(int i = 0; i<3600*5; i++){
		//ѭ�����е�ǰ����  ǰ��ĳ�����
		carhere = runCar;
		Carinroad = 0;
		while(carhere != NULL){
			qq = carhere->here;
			runthiscar(carhere);
			chere = carhere;
			carhere = carhere->next;
			if(chere->prior != NULL && chere->here->location > chere->prior->here->location){
				if(chere == runCar->next)
					runCar = chere;
				swaploc(chere,chere->prior);
			}
			Carinroad++;
		}
		//��һ��������һ���³�
		carlink *newCartoGo = getNewCar();

		if(newCartoGo != NULL){
			if(runCarTail->here->location > 
				newCartoGo->here->safe_length+newCartoGo->here->speed){
					addnewNode(runCarTail, newCartoGo);
					runCarTail = newCartoGo;
					runthiscar(runCarTail);
					Carstart++;
			}
			else{		//�³�����ȴ����б�
				if(freeCar == NULL){
					freeCar = newCartoGo;
					freeCarTail = freeCar;
				}
				else{
					addnewNode(freeCarTail, newCartoGo);
					freeCarTail = newCartoGo;
				}
			}
		}
		else //�ӵȳ��б�ȡ������
		{
			if(freeCar == NULL)
				;
			else{
				carlink *readygo = freeCar;
				if(freeCar->here->location > 
					readygo->here->safe_length+readygo->here->speed)
				{
					if(freeCarTail == freeCar){
						freeCarTail = NULL;
						freeCar = NULL;
					}
					else
						freeCar = freeCar->next;
					addnewNode(runCarTail, readygo);
					Carstart++;
					runCarTail = readygo;
					runthiscar(runCarTail);
				}
			}
		}
		//if(runCar == NULL)
		//	break;
		if(runCar->here->location > 100000){
			carlink *p = runCar;
			runCar = runCar->next;
			if(runCar != NULL)
				runCar->prior = NULL;
			free(p);
			Carisover++;
		}
		/*
		if(i%4 == 0)
		{
			carlink *xx;
			xx = runCar;
			printf(" %d\n",i);
			for(int kk=0; kk<10; kk++){
				if(xx == NULL)
					break;
				if(xx->here->lane == L1)
					printf(" *");
				else if(xx->here->lane == L2)
					printf("* ");
				else
					printf("++");
				printf(" %3d    %.2f   %.2f   %.2f   %d\n",xx->here->num,xx->here->location,xx->here->speed
					,xx->here->nowspeed,xx->here->lane);
				xx = xx->next;
			}
		}
		*/

		//if(i%100 == 0)
		//	getchar();
		if(runCar == NULL)
			break;

	}
	//printf("������%d ��·��������%d  �������ﳵ��%d �������%d ��ʧ���%.2lf\n"
		//,Carstart,Carinroad,Carisover,CarChangeTimes,Carlost);
	printf("%d \n%d  \n%d \n%d \n%.2lf\n"
		,Carstart,Carinroad,Carisover,CarChangeTimes,Carlost);
	fprintf(output,"%d \n%d  \n%d \n%d \n%.2lf\n"
		,Carstart,Carinroad,Carisover,CarChangeTimes,Carlost);
	}
	

	return 0;
}