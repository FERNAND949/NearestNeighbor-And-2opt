/* traveling_salesman */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <math.h>
#include <GL/freeglut.h>

#define MAXCITY 200000
#define swap(a,b) a+=b, b=a-b, a-=b


int ncity;                    /* the total number of cities you must visit*/
double x[MAXCITY],y[MAXCITY]; /* array for position data */
int nroute[MAXCITY]={0};      /* array for route data */


double Dis(double x, double y, double x2, double y2){

	return sqrt(pow(x - x2,2)  + pow(y - y2,2));
}

void resize(int w, int h){

	glViewport(0, 0, w, h);
	glLoadIdentity();
	glOrtho(-w/500, w/500, -h/500, h/500, -1.0, 1.0);

}


void pathroute(){

	int a;

	glClear(GL_COLOR_BUFFER_BIT);
	glLineWidth(1.0f);
	glViewport(0.0,0.0, 400,400);
	glColor3d(1.0,0.0,0.0);
	glBegin(GL_LINE_LOOP);

	for (a = 0; a < ncity; a++) {

		// 225以降のデータ用
		if(ncity>100) glVertex2d(x[nroute[a]]/1000.0, y[nroute[a]]/1000.0);
		else glVertex2d(x[nroute[a]], y[nroute[a]]);
	}

	glEnd();
	glFlush();

}


int main(int argc,char *argv[])
{
  double dis_tot;               /* total distance */

  /* local variables */ 
  FILE *input, *output;
  int i,j,ii,jj,nrnd;
  double rx,ry,rr;
  clock_t cpu_time1,cpu_time2;
  clock_t t[10]={0};
  int time;
  double tave=0.0,ave=1.0;


  /* Output how to use this program*/
  if(argc!=2){
    printf("usage:./a.out filename\n");
    exit(1);
  }


  /* Error output (check City data name) */
  input = fopen(argv[1], "r");
  if(input==NULL){
    printf("no such file or directory\n");
    exit(1);
  }


  /* Read City data */
  fscanf(input,"%d", &ncity);
  for(i = 0; i < ncity; i++){
    fscanf(input,"%lf %lf",&x[i],&y[i]);
  }
  fclose(input);

  
  /* Output City data (for Check) */
  printf("ncity\n");
  printf("%d\n",ncity);
  if(ncity > MAXCITY){
    printf("ncity is too large\n");
    exit(1);
  }
  printf("Initial City Position:\n");
  for(i = 0; i < ncity; i++){
    printf("%d,%lf,%lf\n",i,x[i],y[i]);
  }


  for (time = 0; time < ave; time++) {
	


  /* Stop Watch START*/
  cpu_time1 = clock();


  /*
    ----------------------------------------
    !!
    !! Nearest Neighbor algorithm START!
    !!
    ----------------------------------------
  */
  printf("-------------------------------\n");
  printf("Route Search Algorithm START\n");
  printf("-------------------------------\n");


  /* set initial route*/
//   for (i=0; i < ncity; i++){
//     nroute[i] = i;
//   }

  // 初期位置は一番最初に読み込んだノード
  int prenum;//  親ノード
  double min;//  最小値
  double dist;// 計算途中でのノード間の距離
  int culnum;//  計算回数
  int nodenum;// 現在計算するノードの番号
  int findnode;// 計算してるノードがすでに通ってるか
  int flag;//     計算してるノードがすでに通ってるかの判定

  min       = 10000;
  dist      = 0;
  nroute[0] = 0;// スタートを0に固定
  prenum    = nroute[0];
  flag      = 0;

  // 計算回数
  for(culnum=1; culnum<ncity; culnum++) {

	  printf("-------------------------------\n");
	  printf("cluculate num %d\n",culnum);
	  // 距離を計算するノードの番号
	  for(nodenum = 1; nodenum < ncity; nodenum++){

		  printf("nodenum num %d\n",nodenum);
		  // 計算してるノードがすでに通ったかどうかを確かめる
		  for (findnode = 1; findnode < ncity; findnode++) {
			  if(nroute[findnode] == nodenum){
				 
				  flag = 0;
				  printf("findnode=%d\n",findnode);
				  printf("nroute[%d] nodenum=%d\n",findnode,nodenum);
				  break;
			  }
			  else flag=1;
		  }// findnode

		  // ノードがまだ通っていなければ
		  if(flag){

			  dist = sqrt(pow(x[prenum] - x[nodenum],2)  + pow(y[prenum] - y[nodenum],2));
			  if (dist < min) {
					  
				  nroute[culnum] = nodenum;
				  min = dist;

				  printf("not path, in nroute[%d] %d\n",culnum,nodenum);
				  printf("min=%lf dist=%lf\n",min,dist);
			  }
			  else{
				  printf("dist over mini\n");
			  }
		  }
		  else{
			  printf("already path node\n");
		  }

		  printf("findnode loop out \n");
		  printf("\n");
	  }// nodenum 

	  prenum = nroute[culnum];
	  printf("prenum[%d]=%d\n",culnum,prenum);
	  min = 10000000.0;

	  printf(" \n");
	  printf("-------------------------------\n");

  }// culnum



  /* generate random number */
  /*srand((unsigned)time(NULL));

  for(i=0; i < 100; i++){
    nrnd = rand()%(ncity);
    printf("%d\n",nrnd);
    } */

  printf("-------------------------------\n");
  printf("Route Search Algorithm END\n");
  printf("-------------------------------\n");

  /*
    !!---------------------------------------
    !!
    !! Nearest Neighbor algorithm END!
    !!
    !!---------------------------------------
  */


/*
    !!---------------------------------------
    !!
    !! 2-opt algorithm start!
    !!
    !!---------------------------------------
  */

  printf("-------------------------------\n");
  printf("2-opt Algorithm START\n");
  printf("-------------------------------\n");

  int a,a2,a3;
  double d,d2;

  d=d2=0.0;

  // 元の経路
  for (a3 = 0; a3 < ncity-1;  a3++){

	  d += Dis( x[nroute[a3]],y[nroute[a3]], x[nroute[a3+1]],y[nroute[a3+1]] );
  }
	
  // 入れ替え
  for (a = 0; a < ncity; a++){

// 	  printf("%d\n",a);
	  for (a2 = 0; a2 < ncity+1; a2++){
		  if(a2==ncity){

			  if(a != 0) swap(nroute[a], nroute[0]);
			  else continue;
		  }
		  else if(a==a2) continue;
		  else  swap(nroute[a], nroute[a2]);
		  
		  d2=0.0;

		  // 組み換え後の距離
		  for (a3 = 0; a3 < ncity-1;  a3++){

			  d2 += Dis( x[nroute[a3]],y[nroute[a3]], x[nroute[a3+1]],y[nroute[a3+1]] );
		  }
		  d2 += Dis( x[nroute[a3]],y[nroute[a3]], x[nroute[0]],y[nroute[0]] );

		  if(d2>d){

// 			  printf("not change\n");
			  if(a2==ncity) swap(nroute[a], nroute[0]);
			  else  swap(nroute[a], nroute[a2]);
		  }
		  else {

// 			  printf("change\n");
// 			  printf("node num nroute[a]=%d nroute[a2]=%d\n", nroute[a], nroute[a2]);
// 			  printf("restart for loop \n");
			  d=d2;
			  a=0;
			  break;
		  }
	  }// a2
			  
// 	  printf("%d\n",a);
// 	  printf("\n");
// 	  printf("\n");
  }// a

  printf("-------------------------------\n");
  printf("2-opt Algorithm end\n");
  printf("-------------------------------\n");

/*
    !!---------------------------------------
    !!
    !! 2-opt algorithm END!
    !!
    !!---------------------------------------
  */



  /* Stop Watch END */
  cpu_time2 = clock();


  t[time]=cpu_time2-cpu_time1;

  if(time<ave-1){
	for (a = 0; a < ncity; a++) {
		nroute[a]=0;
	} 
  }
  }


  for(time=0; time<ave; time++){

	  tave += (double)t[time];
  }

  tave = tave/ave;


  /* Output City Data */
  printf("Optimized Route\n");
  for (i=0 ; i < ncity ; i++){
    j= nroute[i];
    printf("%d,%lf,%lf\n",j,x[j],y[j]);
  }


  /* Output Distance */
  printf("\n");
  printf("Total Distance!\n");
  dis_tot = 0.0;
  for(i=0; i < ncity-1; i++){
    ii = nroute[i];
    jj = nroute[i+1];
    rx = x[ii] - x[jj];
    ry = y[ii] - y[jj];
    rr = sqrt(rx*rx + ry*ry);
    dis_tot = dis_tot + rr;
  }
  ii = nroute[ncity-1];
  jj = nroute[0];
  rx = x[ii] - x[jj];
  ry = y[ii] - y[jj];
  rr = sqrt(rx*rx + ry*ry);
  dis_tot = dis_tot + rr;
  printf("%lf\n",dis_tot);


  /* Output GNUPLOT data */
  output = fopen("route.dat","w");
  fprintf(output,"%lf\n",dis_tot);
  fprintf(output,"%20.14f\n",(double)(tave)/(double)CLOCKS_PER_SEC);
  for(i = 0; i < ncity; i++){
    j = nroute[i];
    fprintf(output,"%d %lf %lf\n",j,x[j],y[j]);
  }
  j = nroute[0];
  fprintf(output,"%d %lf %lf\n",j,x[j],y[j]);
  fclose(output);


  /* Output Computational Time */
  printf("\n");
  printf("Elapsed Time: %20.14f sec\n",tave/(double)CLOCKS_PER_SEC);





  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA);
  glutInitWindowSize(500,500);
  glutCreateWindow("route");
  glutDisplayFunc( pathroute );
  glutReshapeFunc(resize);
  glClearColor(1.0, 1.0, 1.0, 1.0);
  glutMainLoop();



  return 0;
}









