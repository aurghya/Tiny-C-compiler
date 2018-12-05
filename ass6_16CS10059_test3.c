int singledigit(int x)       // Recursive function that will return the sum of every single digit
{
    if((x/10)==0)
    {
        return x;
    }
    else
    {
        return (singledigit(x%10)+singledigit(x/10));     // It will calculate the sum of the digits of the number.
    }
}

int main()
{
    int n;
    printStr("This Programs calculate repeated sum of digits of a number.\n");
    printStr("ENTER NUMBER\n");    // To input a number
    int err=1;
    n=readInt(&err);
    while((n/10)!=0)
    {
        n=singledigit(n);     // Calculate the sum of the digits until you get a single digit i.e from 0 to 9 and then check if it is 1
    }
    printStr("\nThe single digit sum of the number is \n");
    printInt(n);
    printStr("\n");
    return 0;
}
