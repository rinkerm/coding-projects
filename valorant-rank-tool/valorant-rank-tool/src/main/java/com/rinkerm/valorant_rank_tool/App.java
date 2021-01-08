package com.rinkerm.valorant_rank_tool;
import java.util.Scanner;
import java.io.File;
public class App 
{
	static boolean createLoginProfile()
	{
		int files = new File("profiles").listFiles().length;
		System.out.println(files);
		return false;
	}
	
	static void printLoginProfiles()
	{
		//prints out usernames of all login profiles
	}
	
	static void printMainMenuOptions()
	{
        System.out.println( "Main Menu:");
        System.out.println( "(c)reate login profile");
        System.out.println( "(v)iew login profiles");
        System.out.println( "(l)ogin");
        System.out.println( "(e)xit");
	}
	
    public static void main( String[] args )
    {
        System.out.println( "======= Thorn's Valorant LP Checker =======");
        printMainMenuOptions();
        Scanner menu_scanner = new Scanner(System.in);
        while(true)
        {
        	String input = menu_scanner.next();
        	if(input.equals("c"))
        	{
        		//Handle Login Profile Creation (Returns true if worked, false if didn't)
        		boolean result = createLoginProfile();
        		if(result)
        		{
        			System.out.println("Profile Creation Successful!");
        			System.out.println("Returning to Menu");
        		}
        		else
        		{
        			System.out.println("Profile Creation Failed!");
        			System.out.println("Please Check Your Login Details.");
        			System.out.println("Returning to Menu");
        		}
        	}
        	else if(input.equals("v"))
        	{
        		printLoginProfiles();
        	}
        	else if(input.equals("l"))
        	{
        		//Will handle login here
        	}
        	else if(input.equals("e")) {
				menu_scanner.close();
        		System.out.println("Exiting...");
        		System.out.println("Goodbye!");
	        	System.exit(0);

        	}
        	else 
        	{
        		System.out.print("You entered: ");
        		System.out.println(input);
        		System.out.println("Please Select an Option from the Menu");
        		printMainMenuOptions();
        	}
        }
    }
}
