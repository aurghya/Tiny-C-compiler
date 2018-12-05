//Longest Common subsequence
int max(int a, int b)
{
	if(a>b)
	{
		return a;
	}    
	else
	{
		return b;
	}
}

int lcs( int X[], int Y[], int m, int n )
{
	if (m == 0 || n == 0)
	{
		return 0;
	}
	if (X[m-1] == Y[n-1])
	{
		return (1 + lcs(X, Y, m-1, n-1));
	}
	else
	{
		return max(lcs(X, Y, m, n-1), lcs(X, Y, m-1, n));
	}
}

int main()
{
	int x[100];
	int y[100];
	int n,m,i;
	int err=1;
	printStr("Program to print the length of longest common subsequence\n");
	printStr("Enter the number of elements in first array\n");
	n=readInt(&err);
	printStr("Enter the elements of the first array one by one\n");
	for(i=0;i<n;i++)
	{
		x[i]=readInt(&err);
	}

	printStr("Enter the number of elements in second array\n");
	m=readInt(&err);
	printStr("Enter the elements of the second array one by one\n");
	for(i=0;i<m;i++)
	{
		y[i]=readInt(&err);
	}
	int len;
	len=lcs(x,y,n,m);
	printStr("Length of longest common subsequence is: ");
	printInt(len);
	printStr("\n");
	return 0;
}
