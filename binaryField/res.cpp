#include "bit.h"
using namespace std;
#define ll long long

class node {

public:
	bit ***a = NULL;
	int x,y,z;
	int sx,sy,sz;
	node():x(0),y(0),z(0),sx(0),sy(0),sz(0){
		this->a = new bit**[1];
		this->a[0] = new bit*[1];
		this->a[0][0] = new bit[1];
		this->a[0][0][0] = 0;
	}

	node(const int &x,const int &y,const int &z):x(x),y(y),z(z),sx(x),sy(y),sz(z)
	{
		//cout<<"Enter constructor : \n";
		bit *s = new bit[10];
		this->a = new bit **[x+1];
		//cout<<"hello 1\n";
		for(int i = 0 ; i <= x; i++)
		{
			this->a[i] = new bit*[y+1];
			//cout<<"hello 2\n";
			for(int j = 0; j <= y; j++)
			{
				//cout<<"hello 4\n";
				this->a[i][j] = new bit[z+1];
				//cout<<"hello 3\n";
				for(int k = 0 ; k <= z; k++)
				{
					//cout<<"hello 3\n";
					this->a[i][j][k] = 0;
				}
			}
		}
	}

	node(const node &b)
	{

		this->x = b.x; this->sx = b.sx;
		this->y = b.y; this->sy = b.sy;
		this->z = b.z; this->sz = b.sz;
		this->a = new bit **[b.sx+1];
		for(int i = 0 ; i <= b.sx; i++)
		{
			this->a[i] = new bit*[b.sy+1];
			for(int j = 0; j <= b.sy; j++)
			{
				this->a[i][j] = new bit[b.sz+1];
				for(int k = 0 ; k <= b.sz; k++)
				{
					this->a[i][j][k] = b.a[i][j][k];
				}
			}
		}
	}

	node& operator=(const node &b)
	{
		if(this != &b)
		{
			for(int i = 0; i <= sx; i++)
	    	{
	    		for(int j = 0; j <= sy; j++)
	    		{
	       			delete [] this->a[i][j];
	    		}
	    		delete [] this->a[i];
	    	}
    		delete [] this->a;

    		this->x = b.x; this->sx = b.sx;
    		this->y = b.y; this->sy = b.sy;
    		this->z = b.z; this->sz = b.sz;
    		this->a = new bit **[b.sx+1];
			for(int i = 0 ; i <= b.sx; i++)
			{
				this->a[i] = new bit*[b.sy+1];
				for(int j = 0; j <= b.sy; j++)
				{
					this->a[i][j] = new bit[b.sz+1];
					for(int k = 0 ; k <= b.sz; k++)
					{
						this->a[i][j][k] = b.a[i][j][k];
					}
				}
			}

		}
		return *this;
	}
	~node()
    {
    
    	for(int i = 0; i <= sx; i++)
    	{
    		for(int j = 0; j <= sy; j++)
    		{
    			delete [] this->a[i][j];
    		}
    		delete [] this->a[i];
    	}
    	delete [] this->a;
    	this->a = NULL;
	}

	void degree(node &b)
	{
		int p = 0,q = 0,r = 0;
		for(int i = b.x; i >= 0 ; i--)
			for(int j = b.y; j >= 0; j--)
				for(int k = b.z; k >= 0; k--)
					if(!(!b.a[i][j][k]))
					{
						p = max(p,i);
						q = max(q,j);
						r = max(r,k);
					}
		b.x = p; 
		b.y = q;
		b.z = r;
	}
	node operator+(const node &b)
	{
		node temp(max(this->x,b.x),max(this->y,b.y),max(this->z,b.z));
		for(int i = 0; i <= this->x; i++)
			for(int j = 0; j <= this->y; j++)
				for(int k = 0; k <= this->z; k++)
					temp.a[i][j][k] = this->a[i][j][k];
		for(int i = 0; i <= b.x; i++)
			for(int j = 0; j <= b.y; j++)
				for(int k = 0; k <= b.z; k++)
					temp.a[i][j][k] += b.a[i][j][k];
		degree(temp);
		return temp;
	}

	node operator-(const node &b)
	{
		node temp(max(this->x,b.x),max(this->y,b.y),max(this->z,b.z));
		for(int i = 0; i <= this->x; i++)
			for(int j = 0; j <= this->y; j++)
				for(int k = 0; k <= this->z; k++)
					temp.a[i][j][k] = this->a[i][j][k];
		for(int i = 0; i <= b.x; i++)
			for(int j = 0; j <= b.y; j++)
				for(int k = 0; k <= b.z; k++)
					temp.a[i][j][k] -= b.a[i][j][k];
		degree(temp);
		return temp;
	}

	node operator*(const node &b)
	{
		node temp(this->x+b.x, this->y+b.y, this->z+b.z);
		for(int i = 0 ; i <= this->x; i++)
			for(int j = 0; j <= this->y; j++)
				for(int k = 0; k <= this->z; k++)
					for(int i1 = 0; i1 <= b.x; i1++)
						for(int j1 = 0; j1 <= b.y; j1++)
							for(int k1 = 0; k1 <= b.z; k1++)
								temp.a[i+i1][j+j1][k+k1] += this->a[i][j][k]*b.a[i1][j1][k1];
		degree(temp);
		return temp;
	}

	static node shift_mul(const node &b, bit x, int p, int q, int r)
	{
		node temp(b.x+p, b.y+q, b.z+r);
		for(int i = 0; i <= b.x; i++)
			for(int j = 0; j <= b.y; j++)
				for(int k = 0; k <= b.z; k++)
					temp.a[i+p][j+q][k+r] = b.a[i][j][k]*x;
		return temp;
	}
	node operator /(const node &b)
	{
		node temp(max(b.x,this->x),max(b.y,this->y),max(b.z,this->z)),temp2(*this);
		int I=-1,J,K;
		for(int i = b.x; i >= 0 ; i--)
			for(int j = b.y; j >= 0; j--)
				for(int k = b.z; k >= 0; k--)
					{
						if(!(!b.a[i][j][k]) && I == -1)
						{
							I = i;
							J = j;
							K = k;
							break;
						}
					}
		int p = this->x, q =  this->y, r = this->z;
		int count = 0;
		while(++count)
		{
			int f = 0,w = 0;
			while(!temp2.a[p][q][r] || p < I || q < J || r < K)
			{
				w = 1;
				if(r > 0) r--;
				else if(q > 0)
				{
					q--;
					r = this->z;
				}
				else if(p > 0)
				{
					p--;
					q = this->y;
					r = this->z;
				}
				else 
				{
					f = 1;
					break;
				}
			}
			//cout<<"count = "<<count<<"\n";
			if(count == 100000)
			{
				cout<<"limit exceeded\n";
				break;
			} 
			//if(count != 1 && !w) break;
			if(f) break;
			if(p < I || q < J || r < K) break;
			//cout<<"p = "<<p<<" q = "<<q<<" r = "<<r<<"\n";
			int i = p - I, j = q - J, k = r - K;
			temp.x = max(temp.x,i);
			temp.y = max(temp.y,j);
			temp.z = max(temp.z,k);
			//cout<<"ikkada 1\n";
			//cout<<b.a[I][J][K]<<"\n";
			//cout << temp2.a[p][q][r] << "\n";
			temp.a[i][j][k] = temp2.a[p][q][r]/b.a[I][J][K];
			//cout<<"ikkada 2\n";
			node c = shift_mul(b,temp.a[i][j][k],i,j,k);
			temp2 = temp2 - c;
		}
		degree(temp);
		return temp;
	}

	node operator %(const node &b)
	{
		node temp(max(b.sx,this->sx),max(b.sy,this->sy),max(b.sz,this->sz)),temp2(*this);
		int I=-1,J,K;
		for(int i = b.x; i >= 0 ; i--)
			for(int j = b.y; j >= 0; j--)
				for(int k = b.z; k >= 0; k--)
					{
						if(!(!b.a[i][j][k]) && I == -1)
						{
							I = i;
							J = j;
							K = k;
							break;
						}
					}
		int p = this->x, q =  this->y, r = this->z;
		int count;
		while(++count)
		{
			int f = 0,w = 0;
			while(!temp2.a[p][q][r] || p < I || q < J || r < K)
			{
				w = 1;
				if(r > 0) r--;
				else if(q > 0)
				{
					q--;
					r = this->z;
				}
				else if(p > 0)
				{
					p--;
					q = this->y;
					r = this->z;
				}
				else 
				{
					f = 1;
					break;
				}
			}
			//cout<<"count = "<<count<<"\n";
			if(count == 10000)
			{
				cout<<"limit exceeded\n";
				break;
			}
			//if(count != 1 && !w) break;
			if(f) break;
			if(p < I || q < J || r < K) break;
			//cout<<"p = "<<p<<" q = "<<q<<" r = "<<r<<"\n";
			int i = p - I, j = q - J, k = r - K;
		    node c = shift_mul(b,temp2.a[p][q][r]/b.a[I][J][K],i,j,k);
		   // cout<<"in here\n"; 
			temp2 = temp2 - c;
			//cout<<"end \n";
		}
		degree(temp2);
		return temp2;
	}

	void print(node r)
	{
		for(int i = 0; i <= r.x; i++)
		{
			for(int j = 0; j <= r.y; j++)
			{
				for(int k = 0; k <= r.z; k++)
				{
					cout<<r.a[i][j][k]<<" ";
				}
				cout<<"\n";
			}
			cout<<"\n\n";
		}
		cout<<"r.x = "<<r.x<<" r.y = "<<r.y<<" r.z = "<<r.z<<" r.sx = "<<r.sx<<" r.sy = "<<r.sy<<" r.sz = "<<r.sz<<"\n";

	}


};

void print(node r)
{
	for(int i = 0; i <= r.x; i++)
	{
		for(int j = 0; j <= r.y; j++)
		{
			for(int k = 0; k <= r.z; k++)
			{
				cout<<r.a[i][j][k]<<" ";
			}
			cout<<"\n";
		}
		cout<<"\n\n";
	}
	cout<<"r.x = "<<r.x<<" r.y = "<<r.y<<" r.z = "<<r.z<<" r.sx = "<<r.sx<<" r.sy = "<<r.sy<<" r.sz = "<<r.sz<<"\n";
}


node PRS(node &a, node &b)
{
	//cout<<"PRS begin\n";
	node r0 = a, r1 = b;
	if(r1.x > r0.x) swap(r0,r1);
	int i = 1;
	node gamma0,gamma1;
	int d0 = 0,d1 = 0;
	node beta,psi;
	//cout<<"loop begin\n";
	while(r1.x > 0)
	{
		//cout<<"i = "<<i<<"\n";
		if(i == 1) d1 = d0 = r0.x - r1.x;
		else d1 = r0.x - r1.x;
		node g(0,0,0);
		g.a[0][0][0] = -1;
		node temp(0,r1.y,r1.z);
		for(int i = 0; i <= r1.y; i++)
			for(int j = 0; j <= r1.z; j++)
				temp.a[0][i][j] = r1.a[r1.x][i][j];
		temp.degree(temp);
		if(i == 1) 
		{
			gamma0 = g*g;
			gamma1 = temp;
		}
		else gamma1 = temp;
		if(i == 1)
		{
			if(d1%2 == 0) beta.a[0][0][0] = -1;
			else beta.a[0][0][0] = 1;
			psi.a[0][0][0] = -1;
		}
		else
		{
			node t2;
			t2 = g*g;
			for(int j = 0; j < d0; j++) t2 = t2*psi;
			node t1 = psi;
			for(int j = 0; j < d0 ; j++) t1 = t1*gamma0;
			if(d0%2) t1 = t1*g;
			//cout<<"psi 1\n";
			psi = t1/t2;
			//cout<<"psi 2\n";
			beta = g*gamma0;
			for(int j = 0; j < d1; j++) beta = beta*psi;
		}
		node result = r0;
		for(int j = 0; j < d1+1; j++) result = result*gamma1;
		//cout<<"here 1\n";
		result  = result%r1;
		//cout<<"here 2\n";
		result = result/beta;
		//cout<<"here 3\n";
		r0 = r1;
		r1 = result;
		gamma0 = gamma1;
		if(i > 1)
		{
			d0 = d1;
		}
		i++;
	}
	if(r1.x < r0.x - 1)
	{
		int d = r0.x - r1.x - 1;
		node result = r1;
		for(int j = 0; j < d; j++) result = result*r1;
		r1 = result;
	}
	//cout<<"PRS end\n";
	return r1;
}

node shift(node &f)
{
	//cout<<"shift start\n";
	node temp(f.y,f.z,0);
	for(int i = 0; i <= f.y; i++)
		for(int j = 0; j <= f.z; j++)
			temp.a[i][j][0] = f.a[0][i][j];
	return temp;
	//cout<<"shift end\n";
}

node f(bit & a, bit & b, bit *v, int n, ll X, ll Y)
{
	//cout<<"n = "<<n<<" X"<<X<<" X"<<Y<<"\n";
	if(n == 2)
	{
		node result(1,1,0);
		result.a[0][0][0] = 0;
		result.a[0][1][0] = 1;
		result.a[1][0][0] = -1;
		result.degree(result);
		return result;
	}
	if(n == 3)
	{
		//cout<<"aaya\n";
		node result(2,2,0);
		result.a[0][0][0] = a*a - bit(4)*b*v[0];
		result.a[0][1][0] = bit(-2)*a*v[0] + bit(-4)*b;
		result.a[0][2][0] = v[0]*v[0];
		result.a[1][0][0] = bit(-2)*(v[0]*a+ bit(2)*b);
		result.a[1][1][0] = bit(-2)*(v[0]*v[0] + a);
		result.a[1][2][0] = bit(-2)*(v[0]);
		result.a[2][0][0] = v[0]*v[0];
		result.a[2][1][0] = bit(-2)*v[0];
		result.a[2][2][0] = 1;
		//cout<<"gaya\n";
		result.degree(result);
		return result;
	}
	node f1,f2;
	ll t = min(n/2 -1,1);
	ll n1 = n - t, n2 = t + 2;;
	//vector<bit> v1,v2;
	//for(ll i = 0; i < n1-2; i++) v1.push_back(v[i]);
	//for(ll i = n1-2; i < n-2; i++) v2.push_back(v[i]);
	bit *v1 = v, *v2 = (v + (n1-1));
	f1 = f(a,b,v1,n1,X,Y+1);
	f2 = f(a,b,v2,n2,Y,Y+1);
	f1.degree(f1);
	f2.degree(f2);
	node r1(f1.x,0,f1.y);
	for(int i = 0; i <= f1.x; i++)
		for(int j = 0; j <= f1.y; j++)
			r1.a[i][0][j] = f1.a[i][j][0];
	r1.degree(r1);
	node r2 = PRS(r1,f2);
	node result = shift(r2);
	return result;
}
node f(bit &a, bit &b, bit *v, int n, ll X)
{
	//cout<<"n = "<<n<<" X"<<X<<"\n";
	if(n == 2)
	{
		node result(1,0,0);
		result.a[1][0][0] = -1;
		result.a[0][0][0] = v[0];
		result.degree(result);
		return result;
	}
	if(n == 3)
	{
		node result(2,0,0);
		result.a[0][0][0] = (v[0]*v[1] - a)*(v[0]*v[1] - a) - bit(4)*b*(v[0]+v[1]);
		result.a[1][0][0] = bit(-2)*((v[0] + v[1])*(v[0]*v[1] + a) + bit(2)*b);
		result.a[2][0][0] = (v[0] - v[1])*(v[0] - v[1]);
		//cout<<"gaya\n";
		result.degree(result);
		//print(result);
		return result;
	}
	node f1,f2;
	ll t = min(n/2 -1,1);
	ll n1 = n - t, n2 = t + 2;;
	//vector<bit> v1,v2;
	//for(ll i = 0; i < n1-1; i++) v1.push_back(v[i]);
	//for(ll i = n1-1; i < n-1; i++) v2.push_back(v[i]);
	bit *v1 = v, *v2 = (v + (n1-1));
	f1 = f(a,b,v1,n1,X+1);
	f2 = f(a,b,v2,n2,X,X+1);
	f1.degree(f1);
	f2.degree(f2);
	node result = PRS(f1,f2);
	result = shift(result);
	return result;
}

node f(bit & a, bit &b, bit *v, int n)
{	
	//cout<<"n = "<<n<<"\n";
	if(n == 2) 
	{
		//cout<<"hello\n";
		node result(0,0,0);
		//cout<<"hello 1\n";
		result.a[0][0][0] = v[0] - v[1];
		//cout<<"hello 2\n";
		return result;
	}
	if(n == 3) 
	{
		node result(0,0,0);
		result.a[0][0][0] = (v[0] - v[1])*(v[0] - v[1])*v[2]*v[2] - bit(2)*((v[0] + v[1])*(v[0]*v[1] + a) + bit(2)*b)*v[2] + ((v[0]*v[1] - a)*(v[0]*v[1] - a) - bit(4)*b*(v[0] + v[1]));
		return result;
	}
	node f1,f2;
	ll t = min(n/2 -1,1);
	ll n1 = n - t, n2 = t + 2;;
	//vector<bit> v1,v2;
	bit *v1 = v, *v2 = (v + (n1-1));
	// for(ll i = 0; i < n1-1; i++) v1.push_back(v[i]);
	// for(ll i = n1-1; i < n; i++) v2.push_back(v[i]);
	f1 = f(a,b,v1,n1,1);
	f2 = f(a,b,v2,n2,1);
	f1.degree(f1);
	f2.degree(f2);
	//cout<<"f1 : "; for(int i = 0; i <= f1.x; i++) cout<<f1.a[i][0][0]<<" "; cout<<"\n";
	//cout<<"f2 : "; for(int i = 0; i <= f2.x; i++) cout<<f2.a[i][0][0]<<" "; cout<<"\n";

	node result = PRS(f1,f2);
	return result;
}

int main(int argc, char const *argv[])
{
	// node q(1,0,0);
	// q.a[1][0][0] = 1;
	// node w(q);
	// cout<<w.a[1][0][0]<<"\n";
	ll n;
	cin>>n;
	vector<ll> g(n);
	for(ll i = 0; i < n; i++) cin>>g[i];
	//cout<<"before \n";
	bit *v = new bit[n];
	//cout<<"hello\n";
	for(int i = 0; i < n; i++) v[i] = g[i];
	//cout<<"v : "; for(int i = 0; i < n; i++) cout<<v[i]<<" "; cout<<"\n";
	bit a(0), b(0);
	clock_t c1, c2;
   	srand((unsigned int)time(NULL));
   	c1 = clock();
   	//cout<<"start\n";
	node z = f(a,b,v,n);
	c2 = clock();
	double di = (double)(c2-c1) / (double)CLOCKS_PER_SEC;
	cout<<z.a[0][0][0]<<"\n";
	printf("+++ Calculation time  = %lf\n", di);
	return 0;
}

// int main(int argc, char const *argv[])
// {
// 	//__m256i *a = new __m256i();
// 	int *a = new int[10];
// }