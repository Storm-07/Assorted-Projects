/**
 * CSC 345-02 Assignment #1 (version 1.2)
 * 
 * On my honor, Nicholas Storm McCrea, this assignment is my own work.  
 * I, Nicholas Storm McCrea, will follow the instructor's rules and processes 
 * related to academic integrity as directed in the course syllabus.
 *
 */

import java.io.File;
import java.io.FileNotFoundException;
import java.util.Scanner;

// Students -- Add your import declarations here

public class LexicalAnalyzer {
	
	// Students -- Add your constants here
	
	public static void main(String[] args) {
		try {
			// Do NOT make any changes to the following TWO lines
			File file = new File(args[0]);		
			Scanner sc = new Scanner(file);		//*** Do not make any other Scanners ***//
			
			// *** NOTE ***
			// For this assignment, you are NOT allowed to use any member methods of 
			// class java.util.Scanner except hasNextLine and nextLine.  
			// For example, you CANNOT use any of hasNextInt, nextInt, hasNextFloat, nextFloat, 
			// hasNextDouble and nextDouble for this assignment.  
			
			// Students -- Your code and methods calls go here
			
			String filecontent = getLine(sc); // one line all file content w/ o spaces
			int token = 0;
			String storeNum = "";

			for(int i = 0; i < filecontent.length(); i++) {
				char currLetter = filecontent.charAt(i); // current lexeme
				token = getToken1(currLetter);
				boolean isADigit = Character.isDigit(currLetter);

				// if a number is being built and the current token is not a digit
				if((storeNum != "") && (token != 69)) {
					String inv = getInValidFloat(storeNum);
					boolean isValidF = isValidFloat(storeNum);
					token = getToken2(storeNum);
					if(isValidF == false) {
						System.out.printf("Next token is: %d, Next lexemme is %s\n", token, inv);
						break;
					}
					else
						System.out.printf("Next token is: %d, Next lexemme is %s\n", token, storeNum);
					storeNum = "";
					token = getToken1(currLetter);
				}

				// if character is a digit store in a string
				if((isADigit == true) && (token == 69) || (currLetter == '.') && (token == 69)) {
					storeNum += currLetter;
				}
				if((token != 69) && storeNum == "") {
				// this line can only print if token != 69
					System.out.printf("Next token is: %d, Next lexemme is %s\n", token, currLetter);
				}
			}
			System.out.println();
			
			sc.close();
		} catch (FileNotFoundException e) {
			System.out.println("ERROR - cannot open front.in \n");
		}
	}
	
	// Students -- Add your method declarations here

	// gets all file content and subtracts all spaces and new lines
	public static String getLine(Scanner sc) { 
		String str = "";

		while(sc.hasNextLine()) {
			str += sc.nextLine();
			str = str.replaceAll("\\s+", "");
			str = str.replaceAll("[\n\r]", "");
		}
		return str;
	}

	// get token of single case lexemes
	public static int getToken1(char c) {
		int token = 0;
		switch(c) {
			case 'f':
				token = 0;
				break;
			case 'i':
				token = 1;
				break;
			case 'p':
				token = 2;
				break;
			case '=':
				token = 4;
				break;
			case '+':
				token = 5;
				break;
			case '-':
				token = 6;
				break;
			default:
				token = 69; // this is numbers and unknown chars
				break;
		}

		// check identifier
		if(c == 'a' || c == 'b' || c == 'c' || c == 'd' || c == 'e' || c == 'g' || c == 'h'
		|| c == 'j' || c == 'k' || c == 'l' || c == 'm' || c == 'n' || c == 'o' || c == 'q'
		|| c == 'r' || c == 's' || c == 't' || c == 'u' || c == 'v' || c == 'w' || c == 'x'
		|| c == 'y' || c == 'z')
			token = 3;
		return token;
	} // if 69 is returned, and the character is a digit, then token = 3 (for main)

	// deal with integers and floats
	public static int getToken2(String str) {
		int token = 0;
		if(str.contains("."))
			token = 8;
		else
			token = 7;
		
		return token;
	}

	// check for valid float values
	public static boolean isValidFloat(String str) {
		boolean isValid = true;
		int count = 0;
		for(int i = 0; i < str.length(); i++) {
			char currChar = str.charAt(i);
			if(currChar == '.')
				count++;
		}

		if(count > 1)
			isValid = false;
		return isValid;
	}

	public static String getInValidFloat(String str) {
		String inv = "";
		int secDotIdx = 0;
		int count = 0;
		for(int i = 0; i < str.length(); i++) {
			char currChar = str.charAt(i);
			if(currChar == '.')
				count++;
			if(count == 2)
				secDotIdx = i - 1;
		}

		inv = str.substring(0, secDotIdx);
		return inv;
	}
}
