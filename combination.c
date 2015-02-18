/* traveling_salesman */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <float.h>

#define MAXCITY 15

double total_distance(int ncity,int nroute[],double x[],double y[]){
  int i,ii,jj;
  double rx,ry,rr,dis_tot;

  dis_tot = 0.0;

  for(i=0; i < ncity; i++){
    ii = nroute[i];
    jj = nroute[i+1];
    rx = x[ii] - x[jj];
    ry = y[ii] - y[jj];
    rr = sqrt(rx*rx + ry*ry);
    dis_tot += rr;
  }
  return dis_tot;
}

int main(int argc,char *argv[])
{
  int ncity; /* the total number of cities you must visit*/
  double x[MAXCITY],y[MAXCITY]; /* array for position data */
  int minroute[MAXCITY+1]={0}; /* array for route data */
  int croute[MAXCITY-1]={0}; /* array for route data */
  int proute[MAXCITY-1]={0}; /* array for route data */
  double dis_tot; /* total distance */

  /* local variables */ 
  FILE *input, *output;
  int i,j,ii,jj;
  int count,ccity,cpos,qpos,nchange,nwork;
  double dis_ij,dis_min;
  double rx,ry,rr;
  clock_t cpu_time1,cpu_time2;

  /* check input*/
  if(argc!=2){
    printf("usage:./a.out filename\n");
    exit(1);
  }

  /* Read City data */
  input = fopen(argv[1], "r");
  if(input==NULL){
    printf("no such file or directory\n");
    exit(1);
  }

  fscanf(input,"%d", &ncity);
  for(i = 0; i < ncity; i++){
    fscanf(input,"%lf %lf",&x[i],&y[i]);
  }
  fclose(input);
  
  /* Output City data (Check) */
  printf("ncity= %d\n",ncity);
  if(ncity > MAXCITY){
    printf("ncity is too large\n");
    exit(1);
  }
  
  printf("city position (input):\n");
  for(i = 0; i < ncity; i++){
    printf("%d,%lf,%lf\n",i,x[i],y[i]);
    if(x[i]<0.0){
      printf("x[%d] is too small\n",i);
      exit(1);
    }else if(x[i]>1.0){
      printf("x[%d] is too large\n",i);
      exit(1);
    }else if(y[i]<0.0){
      printf("y[%d] is too small\n",i);
      exit(1);
    }else if(y[i]>1.0){
      printf("y[%d] is too large\n",i);
      exit(1);
    }
  }

  /* Stop Watch START*/
  cpu_time1 = clock();

  printf("-------------------------------\n");
  printf("Route Search Algorithm START\n");
  printf("-------------------------------\n");

  /* set initial route*/
  for (i=0; i < ncity; i++){
    croute[i] = i;
  }
  croute[ncity] = 0;

  dis_min = DBL_MAX;

  count = 0;
  while(1){
    dis_tot = total_distance(ncity,croute,x,y);
    if(dis_min >= dis_tot){
      dis_min = dis_tot;
      for (i=0; i <= ncity; i++){
	minroute[i] = croute[i];
      }
    }
    //printf("%d: ",count);
    for (i=0; i <= ncity; i++){
      //printf("%d ",croute[i]);
      proute[i] = croute[i];
    }
    //printf("\n");
    qpos = ncity-1;
    for(i=1;i<(ncity-1);i++){
      if(croute[i]<croute[i+1]){
	qpos = i;
	break;
      }
    }

    if(qpos==(ncity-1)){
      break;
    }else{
      cpos = qpos+1;
      ccity = croute[cpos];
      for(i=0;i<cpos;i++){
	for(j=1;j<cpos;j++){
	  if(croute[j]<croute[j+1]){
	    nwork = croute[j+1];
	    croute[j+1] = croute[j];
	    croute[j] = nwork;
	  }
	}
      }

      for(i=1;i<=cpos;i++){
	if(ccity==croute[i]){
	  if(i==cpos){
	    printf("find city is fail\n");
	    exit(1);
	  }
	  break;
	}
      }

      nwork = croute[i+1];
      croute[i+1] = croute[cpos];
      croute[cpos] = nwork;

      for(i=0;i<cpos;i++){
	for(j=1;j<(cpos-1);j++){
	  if(croute[j]>croute[j+1]){
	    nwork = croute[j+1];
	    croute[j+1] = croute[j];
	    croute[j] = nwork;
	  }
	}
      }
    }
    count++;
  }


  printf("-------------------------------\n");
  printf("Route Search Algorithm END\n");
  printf("-------------------------------\n");


  /* Stop Watch END */
  cpu_time2 = clock();
  
  /* Output City Data */
  printf("Optimized Route\n");
  for (i=0 ; i <= ncity ; i++){
    j= minroute[i];
    printf("%d,%lf,%lf\n",j,x[j],y[j]);
  }

  /* Output Distance */
  printf("Total Distance\n");
  dis_tot = total_distance(ncity,minroute,x,y);
  printf("%lf\n",dis_tot);

  /* Output GNUPLOT data */
  output = fopen("route.dat","w");
  fprintf(output,"%lf\n",dis_tot);
  fprintf(output,"%20.14f\n",(double)(cpu_time2-cpu_time1)/(double)CLOCKS_PER_SEC);
  for(i = 0; i <= ncity; i++){
    j = minroute[i];
    fprintf(output,"%d %lf %lf\n",j,x[j],y[j]);
  }
  fclose(output);

  /* Output Computational Time */
  printf("\n");
  printf("Elapsed Time: %20.14f sec\n",(double)(cpu_time2-cpu_time1)/(double)CLOCKS_PER_SEC);

  return 0;
}









