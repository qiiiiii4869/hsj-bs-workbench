#include <cmath>
#include <cstdio>
using namespace std;
double d_dd(int n,double x[],double y[],double dy[],double dd,double &xx);
void oncam (int nb,int bpoint[],double *l[],double *q[],double *eta[],double *dq[],
			double *de[],double qc[],double ec[],double dec[]);
void smooth (int n,double x[],double y[],double dy[],double s,
			 double a[],double b[],double c[],double d[]);
void point (FILE *fp,double x,double y);
void akurve2 (FILE *fp,int n,double x[],double y[]);
double sp2(double x,double x1,double x2,double y1,double y2,
		   double dy1,double dy2);
double sp2d1(double x,double x1,double x2,double y1,
			 double y2,double dy1,double dy2);
double spinte(int n,double xx,double x[],double y[],double dy[]);
double dspinte(int n,double xx,double x[],double y[],double dy[]);
void quasp2 (int n,double x[],double y[],double dy[]);
double d_dl (int n,double l[],double x[],double y[],double dx[],double dy[],double dd,double &ll);
void quasp2l(int n,double x[],double y[],double l[],double dx[],double dy[]);
double cll(double x,double y);
double d_dd(int n,double x[],double y[],double dy[],double dd,double &xx)
{
	double x1=0,x2=0,x3=0,f1=0,f2=0,f3=0,err;
	int i; int budget=10000;

	
	for (i=0;i<n;i++) {  
		if (dy[i]==dd) {
			xx=x[i];
			return (y[i]);
		}
	}

	
	for (i=0;i<n-1;i++) {
		if ((dd-dy[i])*(dy[i+1]-dd)>0.0) {
			x1=x[i];
			x2=x[i+1];
			f1=dy[i]-dd;
			f2=dy[i+1]-dd;
			break;
		}
	}

	
	

	
	if(i>=n-1) throw 2; err=1e5;
	while (budget-- > 0 && err>0.0000001) {
		err=fabs(x2-x1);
		x3=(x1+x2)/2.;
		f3=dspinte(n,x3,x,y,dy)-dd;
		xx=x3;
		if (f1*f3<0.) {
			x2=x3;
			f2=f3;
		}
		else {
			x1=x3;
			f1=f3;
		}
	}
	return (spinte(n,x3,x,y,dy));
}


void oncam (int nb,int bpoint[],double *l[],double *q[],double *eta[],double *dq[],
			double *de[],double qc[],double ec[],double dec[])
{
	int i; int budget=10000;
	double err,t,t1;

	
	
	
	

	
	for (i=0;i<nb;i++) {
		ec[i]=d_dl (bpoint[i],l[i],q[i],eta[i],dq[i],de[i],0.,t);
		qc[i]=spinte (bpoint[i],t,l[i],q[i],dq[i]);
	}

	
	err=1e9;
	while (budget-- > 0 && err>0.000001) {
		quasp2 (nb,qc,ec,dec);
		err=0.;
		for (i=0;i<nb;i++) {
			t1=qc[i];
			ec[i]=d_dl (bpoint[i],l[i],q[i],eta[i],dq[i],de[i],dec[i],t);
			qc[i]=spinte (bpoint[i],t,l[i],q[i],dq[i]);
			if (err<fabs(t1-qc[i])) err=fabs(t1-qc[i]);
		}
	}
}


void smooth (int n,double x[],double y[],double dy[],double s,
			 double a[],double b[],double c[],double d[])
{
	double t[50],t1[50],r[51],r1[51],r2[51],u[51],v[51];
	double p,h,f,g,e,gg,w,w1,w2;
	int k,n1,i;
	
	
	

	r2[0]=0.;
	r2[n-1]=0.;
	r2[n]=0.;
	r1[0]=0.;
	r1[n-1]=0.;
	r[0]=0.;
	u[n-1]=0.;
	u[n]=0.;
	u[0]=0.;
	d[0]=0.;
	p=0.;
	k=0;
	n1=n-1;
	h=x[1]-x[0];
	f=(y[1]-y[0])/h;
	for (i=1;i<n1;i++) {
		g=h;
		e=f;
		h=x[i+1]-x[i];
		f=(y[i+1]-y[i])/h;
		a[i]=f-e;
		t[i]=(g+h)*2.0/3.;
		t1[i]=h/3.0;
		r2[i]=dy[i-1]/g;
		r[i]=dy[i+1]/h;
		r1[i]=-dy[i]/g-dy[i]/h;
	}
	for(i=1;i<n1;i++) {
		b[i]=r[i]*r[i]+r1[i]*r1[i]+r2[i]*r2[i];
		c[i]=r[i]*r1[i+1]+r1[i]*r2[i+1];
		d[i]=r[i]*r2[i+2];
	}
loop:
	k=k+1; if(k>10000) throw 1;
	for (i=1;i<n1;i++) {
		if (i==1) {
			r[i]=1./(b[i]+p*t[i]);
			u[i]=a[i];
		}
		else {
			g=d[i-2];
			f=c[i-1]+p*t1[i-1]-g*r1[i-2];
			r1[i-1]=f*r[i-1];
			r2[i-2]=g*r[i-2];
			r[i]=1./(b[i]+p*t[i]-f*r1[i-1]-g*r2[i-2]);
			u[i]=a[i]-r1[i-1]*u[i-1]-r2[i-2]*u[i-2];
		}
	}
	for (i=n1-1;i>=1;i--) {
		u[i]=r[i]*u[i]-r1[i]*u[i+1]-r2[i]*u[i+2];
	}
	e=0.;
	h=0.;
	for (i=0;i<n1;i++) {
		g=h;
		h=(u[i+1]-u[i])/(x[i+1]-x[i]);
		v[i]=(h-g)*dy[i];
		e=e+v[i]*v[i];
	}
	v[n-1]=u[n1-1]/(x[n-1]-x[n1-1])*dy[n-1];
	e=e+v[n-1]*v[n-1];
	if (e-s<=1e-04) goto label20;
	f=0.;
	gg=0.;
	w1=0.;
	w2=0.;
	for (i=1;i<n1;i++) {
		g=u[i-1]*t1[i-1]+u[i+1]*t1[i]+u[i]*t[i];
		f=f+g*u[i];
		if (i>1) w=g-r1[i-1]*w1-r2[i-2]*w2;
		else w=g;
		w2=w1;
		w1=w;
		gg=gg+w*w*r[i];
	}
	p=p+(e-sqrt(s*e))/(f-p*gg);
	if (p<0.0) {
		s=s*0.8;
		p=0.;
	}
	goto loop;

label20:
	for (i=0;i<n;i++) {
		a[i]=y[i]-dy[i]*v[i];
		c[i]=p*u[i];
	}
	for (i=0;i<n1;i++) {
		h=x[i+1]-x[i];
		d[i]=(c[i+1]-c[i])/3./h;
		b[i]=(a[i+1]-a[i])/h-(h*d[i]+c[i])*h;
	}
	b[n-1]=b[n1-1]+2.*c[n1-1]*h+3.*d[n1-1]*h*h;
}



void point (FILE *fp,double x,double y)
{
	fprintf (fp,"point %f,%f\n",x,y);
}


void akurve2 (FILE *fp,int n,double x[],double y[])
{
	int i,j;

	i=0;
	fprintf (fp,"spline ");
	for (j=0;j<n;j++) {
		fprintf (fp,"%f,%f",x[j],y[j]);
		if (i==1||j==n-1) fprintf (fp,"\n");
		else fprintf (fp," ");
		i++;
		if (i==2) i=0;
	}
	fprintf (fp,"\n\n\n");
}




double sp2(double x,double x1,double x2,double y1,double y2,
		   double dy1,double dy2)
{
	double h,t,sp;

	h=x2-x1;
	t=x-x1;
	if (t<=0.5*h)
		sp=y1+t*(dy1+0.5*t*(4.0*(y2-y1)/pow(h,2.0)-(dy2+3.0*dy1)/h));
	else {
		t=h-t;
		sp=y2+t*(-dy2+0.5*t*((3.0*dy2+dy1)/h-4.0*(y2-y1)/pow(h,2.0)));
	}
	return(sp);
}


double sp2d1(double x,double x1,double x2,double y1,
			 double y2,double dy1,double dy2)
{
	double h,t,dy;

	h=x2-x1;
	t=(x-x1)/h;
	if (t<=0.5)
		dy=dy1+t*(4.*(y2-y1)/h-dy2-3.*dy1);
	else
		dy=-dy1-2.*dy2+4.*(y2-y1)/h+t*(4.*(y1-y2)/h+3.*dy2+dy1);
	return (dy);
}


double spinte(int n,double xx,double x[],double y[],double dy[])
{
	double re;
	int i; int budget=10000;

	if (xx<x[0] && xx>1.1*x[0]-0.1*x[1])
		return (y[0]+(xx-x[0])*dy[0]);
	else if (xx>x[n-1] && xx<1.1*x[n-1]-0.1*x[n-2])
		return (y[n-1]+(xx-x[n-1])*dy[n-1]);
	else {
		for (i=0;i<n-1;i++){
			if (xx>=x[i]&&xx<=x[i+1]){
				re=sp2 (xx,x[i],x[i+1],y[i],y[i+1],dy[i],dy[i+1]);
				return(re);
			}
		}
	}
	return(-1e99);
}


double dspinte(int n,double xx,double x[],double y[],double dy[])
{
	double re;
	int i; int budget=10000;

	for (i=0;i<n-1;i++){
		if (xx>=x[i]&&xx<=x[i+1]){
			re=sp2d1(xx,x[i],x[i+1],y[i],y[i+1],dy[i],dy[i+1]);
			return(re);
		}
	}
	if (i==n-1) re=-1e99;
	return(re);
}


void quasp2 (int n,double x[],double y[],double dy[])
{
	int n1,j;
	double b[100],h,h1,yy,yy1,af,bt;

	if (n==1) dy[0]=0.;
	else if (n==2) {
		dy[0]=(y[1]-y[0])/(x[1]-x[0]);
		dy[1]=dy[0];
	}
	else {
		h=x[1]-x[0];
		yy=y[1]-y[0];
		dy[0]=-.333333333;
		b[0]=1.33333333*yy/h;
		n1=n-1;
		for (j=1;j<n1;j++) {
			h1=h;
			yy1=yy;
			h=x[j+1]-x[j];
			yy=y[j+1]-y[j];
			af=h1/(h1+h);
			bt=4.*((1.-af)*yy1/h1+af*yy/h);
			dy[j]=-af/(3.+(1.-af)*dy[j-1]);
			b[j]=(bt-(1.-af)*b[j-1])/(3.+(1.-af)*dy[j-1]);
		}
		dy[n-1]=(4.*yy/h-b[n-2])/(3.+dy[n-2]);
		for (j=n1-1;j>=0;j--) dy[j]=dy[j]*dy[j+1]+b[j];
	}
}

double d_dl (int n,double l[],double x[],double y[],double dx[],double dy[],double dd,double &ll) {

	double l1=0,l2=0,l3=0,f1=0,f2=0,f3=0,err,df;
	int i; int budget=10000;

	
	for (i=0;i<n;i++) {  
		if (dy[i]/dx[i]==dd) {
			ll=l[i];
			return (y[i]/dx[i]);
		}
	}

	
	for (i=0;i<n-1;i++) {
		if ((dd-dy[i]/dx[i])*(dy[i+1]/dx[i+1]-dd)>0.0) {
			l1=l[i];
			l2=l[i+1];
			f1=dy[i]/dx[i]-dd;
			f2=dy[i+1]/dx[i+1]-dd;
			break;
		}
	}

	
	if(i>=n-1) throw 2; err=1e5;
	while (budget-- > 0 && err>0.0000001) {
		err=fabs(l2-l1);
		l3=(l1+l2)/2.;
		f3=dspinte(n,l3,l,y,dy)/dspinte(n,l3,l,x,dx)-dd;
		ll=l3;
		if (f1*f3<0.) {
			l2=l3;
			f2=f3;
		}
		else {
			l1=l3;
			f1=f3;
		}
	}
	return (spinte(n,l3,l,y,dy));
}

void quasp2l(int n,double x[],double y[],double l[],double dx[],double dy[])
{
	int i; int budget=10000;
	l[0]=0.0;
	for (i=1;i<n;i++) {
		l[i]=l[i-1]+cll(x[i]-x[i-1],y[i]-y[i-1]);
	}
	quasp2(n,l,x,dx);
	quasp2(n,l,y,dy);
}

double cll(double x,double y)
{
	double re;
	re=sqrt(pow(x,2.)+pow(y,2.));
	return(re);
}

int main(){
 try{
 int nb,n[20];double q[20][30],eta[20][30],l[20][30],dq[20][30],de[20][30];
 double a[30],b[30],c[30],d[30],delta[30],qc[20],ec[20],dec[20];
 double *lp[20],*qp[20],*ep[20],*dqp[20],*dep[20];
 if(scanf("%d",&nb)!=1||nb<2||nb>20)return 2;
 for(int i=0;i<nb;i++){
  if(scanf("%d",&n[i])!=1||n[i]<3||n[i]>30)return 3;
  for(int j=0;j<n[i];j++)if(scanf("%lf%lf",&q[i][j],&eta[i][j])!=2)return 4;
  quasp2l(n[i],q[i],eta[i],l[i],dq[i],de[i]);
  for(int j=0;j<30;j++)delta[j]=(i==nb-1?.001:.002);
  smooth(n[i],l[i],eta[i],delta,n[i],a,b,c,d);
  for(int j=0;j<n[i];j++)eta[i][j]=a[j];
  for(int j=0;j<30;j++)delta[j]=(q[i][n[i]-1]-q[i][0])/100.;
  smooth(n[i],l[i],q[i],delta,n[i],a,b,c,d);
  for(int j=0;j<n[i];j++)q[i][j]=a[j];
  quasp2l(n[i],q[i],eta[i],l[i],dq[i],de[i]);
  lp[i]=l[i];qp[i]=q[i];ep[i]=eta[i];dqp[i]=dq[i];dep[i]=de[i];
 }
 oncam(nb,n,lp,qp,ep,dqp,dep,qc,ec,dec);
 for(int i=0;i<nb;i++)for(int j=0;j<100;j++){
  double t=l[i][0]+(l[i][n[i]-1]-l[i][0])*j/99.;
  printf("%.17g %.17g\n",spinte(n[i],t,l[i],q[i],dq[i]),spinte(n[i],t,l[i],eta[i],de[i]));
 }
 for(int j=0;j<100;j++){double t=qc[0]+(qc[nb-1]-qc[0])*j/99.;printf("%.17g %.17g\n",t,spinte(nb,t,qc,ec,dec));}
 return 0;
 }catch(...){return 5;}
}
