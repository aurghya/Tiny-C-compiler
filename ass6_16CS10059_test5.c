
// int main()
// {	
// 	printStr("Program for dot product : \n");
// 	int a[10];
// 	int b[10];
// 	int p, n, x, i, y, z;
// 	printStr("Enter the number of elements of arrays a and b: ");	
// 	n = readInt(&p);
// 	printStr("Enter the elements of array a:\n");
// 	for(i = 0; i < n; i++ )
// 	{
// 		x = readInt(&p);
// 		a[i] = x;
// 	}
// 	printStr("Enter the elements of array b:\n");
// 	for(i = 0; i < n; i++ )
// 	{
// 		x = readInt(&p);
// 		b[i] = x;
// 	}
// 	printStr("The elements of the product array are:\n");
// 	for( i = 0; i < n; i++ )
// 	{
// 		x = a[i];	
// 		y = b[i];
// 		z = x * y;
// 		printInt(z);
// 		printStr(" ");
// 	}
// 	printStr("\n");
// 	return 0;
// }

/************QUICK SORT TEST FILE***************/
/*-----------RECURSIVE APPROACH----------------*/
/* 	The following tests the sorting of an array of 10 user
	fed integers using the quicksort sorting technique*/
int quickSort (int a[],int lo,int hi)
{
	int r ;
    int i=lo, j=hi, t;
    int x,h;
    r = lo+hi;
    r = r/2;
    x = a[r];
    do
    {
    	r = a[i];
        while (r<x)
        {
        	i++; 
        	r = a[i];
        }
        r = a[j];
        while (r>x)
        {
        	j--;
        	r = a[j];
        }
        
    	if (i<=j)
        {
            h=a[i];
            r = a[j];
            a[i]=r;
            a[j]=h;
            i++; j--;
        }
    } while (i<=j);
    
    if (lo<j) quickSort(a, lo, j);
    if (i<hi) quickSort(a, i, hi);
}
int main()
{
	int c,n,i,r;
	int*ep;
	ep = &c;
	int a[100];
	printStr("");
	printStr("--------------QUICKSORT TEST FILE---------------");
	printStr("Here we sort an array of elements"); 
	printStr("using recursive approach:");
	printStr("Enter the 10 elements of array ");
	for(i=0;i<10;i++)
	{
		r = readInt(ep);
		a[i] = r;
	}
	quickSort(a,0,9);
	printStr("The sorted array (in ascending order) follows :");
	for(i=0;i<10;i++)
	{
		r = a[i];
		printInt(r);
		printStr("");
	}
}
