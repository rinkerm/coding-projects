import java.util.Scanner;

public class Integer_to_Roman_Numeral {
	
	static String iterative_numerals(int n)
	{
		String temp = "";
		int number = n;
		while(number>0)
		{
			if(number>=1000) 
			{
				temp = temp + "M";
				number -= 1000;
			}
			else if(number >= 900)
			{
				temp = temp + "CM";
				number -= 900;
			}
			else if(number >= 500)
			{
				temp = temp + "D";
				number -= 500;
			}
			else if(number >= 400)
			{
				temp = temp + "CD";
				number -= 400;
			}
			else if(number>=100)
			{
				temp = temp + "C";
				number -= 100;
			}
			else if(number>=90)
			{
				temp = temp + "XC";
				number -= 90;
			}
			else if(number>=50)
			{
				temp = temp + "L";
				number -= 50;
			}
			else if(number>=40)
			{
				temp = temp + "XL";
				number -= 40;
			}
			else if(number>=10)
			{
				temp = temp + "X";
				number -=10;
			}
			else if(number>=9)
			{
				temp = temp + "IX";
				number -=9;
			}
			else if(number>=5)
			{
				temp = temp + "V";
				number-=5;
			}
			else if(number>=4)
			{
				temp = temp + "IV";
				number-=5;
			}
			else
			{
				temp = temp + "I";
				number-=1;
			}
		}
		return(temp);
	}
	static String numerals(int n, String s)
	{
		String temp = s;
		int number = n;
		if(number>=1000) 
		{
			temp = temp + "M";
			number -= 1000;
			return(numerals(number,temp));
		}
		else if(number >= 900)
		{
			temp = temp + "CM";
			number -= 900;
			return(numerals(number,temp));
		}
		else if(number >= 500)
		{
			temp = temp + "D";
			number -= 500;
			return(numerals(number,temp));
		}
		else if(number >= 400)
		{
			temp = temp + "CD";
			number -= 400;
			return(numerals(number,temp));
		}
		else if(number>=100)
		{
			temp = temp + "C";
			number -= 100;
			return(numerals(number,temp));
		}
		else if(number>=90)
		{
			temp = temp + "XC";
			number -= 90;
			return(numerals(number,temp));
		}
		else if(number>=50)
		{
			temp = temp + "L";
			number -= 50;
			return(numerals(number,temp));
		}
		else if(number>=40)
		{
			temp = temp + "XL";
			number -= 40;
			return(numerals(number,temp));
		}
		else if(number>=10)
		{
			temp = temp + "X";
			number -=10;
			return(numerals(number,temp));
		}
		else if(number>=9)
		{
			temp = temp + "IX";
			number -=9;
			return(numerals(number,temp));
		}
		else if(number>=5)
		{
			temp = temp + "V";
			number-=5;
			return(numerals(number,temp));
		}
		else if(number>=4)
		{
			temp = temp + "IV";
			number-=5;
			return(numerals(number,temp));
		}
		else if(number>=1)
		{
			temp = temp + "I";
			number-=1;
			return(numerals(number,temp));
		}
		else
		{
			return(temp);
		}
	}
	
	public static void main(String[] args) 
	{
		Scanner sc = new Scanner(System.in);
		System.out.println("Enter a number to be converted into Roman Numerals: ");
		int n = sc.nextInt();
		sc.close();
		System.out.print("Recursive Result: ");
		System.out.println(numerals(n,""));
		System.out.print("Iterative Result: ");
		System.out.println(iterative_numerals(n));	
	}
}
