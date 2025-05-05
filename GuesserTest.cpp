/**
 * Unit Tests for the class
**/

#include <gtest/gtest.h>
#include "Guesser.h"

class GuesserTest : public ::testing::Test
{
	protected:
		GuesserTest(){} //constructor runs before each test
		virtual ~GuesserTest(){} //destructor cleans up after tests
		virtual void SetUp(){} //sets up before each test (after constructor)
		virtual void TearDown(){} //clean up after each test, (before destructor)
};

//-------------------- Testing Constructor -----------------------------

//Empty String
TEST(GuesserTest, con_emptyString){
	Guesser guessTest = Guesser("");

	ASSERT_EQ(3, guessTest.remaining()); // Check starting guesses
	ASSERT_EQ(true, guessTest.match("")); // Check string is correct
}

//Full length string. 
TEST(GuesserTest, con_maxLenString){
	Guesser guessTest = Guesser("123456ABCDEFGHIJKLMNOPQRSTUVWXYZ");

	ASSERT_EQ(3, guessTest.remaining()); // Check starting guesses
	ASSERT_EQ(true, guessTest.match("123456ABCDEFGHIJKLMNOPQRSTUVWXYZ")); // Check string is correct

}

//Oversize String, should cut string off past 32 chars
TEST(GuesserTest, con_oversizeString){
	Guesser guessTest = Guesser("123456ABCDEFGHIJKLMNOPQRSTUVWXYZ123");

	ASSERT_EQ(3, guessTest.remaining()); // Check starting guesses
	ASSERT_EQ(true, guessTest.match("123456ABCDEFGHIJKLMNOPQRSTUVWXYZ")); // Check string is correct

}

//String of special characters
TEST(GuesserTest, con_specialCharString){
	Guesser guessTest = Guesser("!@#$%^&*()_+");

	ASSERT_EQ(3, guessTest.remaining()); // Check starting guesses
	ASSERT_EQ(true, guessTest.match("!@#$%^&*()_+")); // Check string is correct
}

TEST(GuesserTest, con_emoji){
	Guesser guessTest = Guesser("☺");

	ASSERT_EQ(3, guessTest.remaining()); // Check starting guesses
	ASSERT_EQ(true, guessTest.match("☺")); // Check string is correct
}

//------------------------- Testing match(); -----------------------------

// Testing lockout system does not trigger at dist=2
TEST(GuesserTest, match_earlyLockout){
	Guesser guessTest = Guesser("ABCD");

	guessTest.match("ABAA"); // dist = 2, should not trigger lockout
	ASSERT_EQ(true, guessTest.match("ABCD")); // Correct guess
}

// Testing lockout system does trigger at dist = 3
TEST(GuesserTest, match_lockout){
	Guesser guessTest = Guesser("ABCD");

	guessTest.match("AAAA"); // dist = 3, should trigger  lockout
	ASSERT_EQ(false, guessTest.match("ABCD")); // Correct guess, but system is locked out
}

// Testing lockout system still triggers past dist = 3
TEST(GuesserTest, match_pastLockout){
	Guesser guessTest = Guesser("ABCDEF");

	guessTest.match("XXXXXX"); // dist = 6, should trigger lockout
	ASSERT_EQ(false, guessTest.match("ABCD")); // Correct guess, but system is locked out
}

// Testing guess decrement lockout system
TEST(GuesserTest, match_outOfGuesses){
	Guesser guessTest = Guesser("ABCD");

	guessTest.match("ABC"); // Incorrect guess 1
	guessTest.match("ABC"); // Incorrect guess 2
	guessTest.match("ABC"); // Incorrect guess 3
	ASSERT_EQ(false, guessTest.match("ABCD")); // Out of guesses, correct guess should not work
}

// Testing for Case Sensitivity
TEST(GuesserTest, match_caseSens){
	Guesser guessTest = Guesser("ABCD");

	guessTest.match("abcD"); // Incorrect guess, should trigger Lockout
	ASSERT_EQ(false, guessTest.match("ABCD")); // Check for lockout
}

// Testing empty string, should not be able to lockout
TEST(GuesserTest, match_emptyString){
	Guesser guessTest = Guesser("");

	guessTest.match("123456ABCDEFGHIJKLMNOPQRSTUVWXYZ"); // Incorrect guess, no lockout
	ASSERT_EQ(true, guessTest.match("")); // Check for no lockout
}

// Test to make sure special characters are recoginized properly
TEST(GuesserTest, match_specialChars){
	Guesser guessTest = Guesser("!@#$%^&*()_+ ");

	ASSERT_EQ(false, guessTest.match(") (%$@(*&@%))")); // Incorrect guess, different chars, same len
}

// Test to make sure order of chars doesn't effect anything
TEST(GuesserTest, match_mixedOrder){
	Guesser guessTest = Guesser("ABCD");

	ASSERT_EQ(false, guessTest.match("DCBA")); // Check same letters, different order
}

// Test to check it can recognize different emojis
TEST(GuesserTest, match_diffEmojis){
	Guesser guessTest = Guesser("😓");

	ASSERT_EQ(false, guessTest.match("🤗")); // Check different emoji
}

// Test empty guess
TEST(GuesserTest, match_emptyGuess){
	Guesser guessTest = Guesser("ABCD");

	guessTest.match(""); // Incorrect, trigger lockout
	ASSERT_EQ(false, guessTest.match("ABCD")); // Check for lockout
}



//------------------------- Testing remaining();-----------------------------

// Check correct initial guesses
TEST(GuesserTest, remain_startingGuesses){
	Guesser guessTest = Guesser("ABCD");

	ASSERT_EQ(3, guessTest.remaining());
}

// Check no decrement on correct guess
TEST(GuesserTest, remain_correctGuess){
	Guesser guessTest = Guesser("ABCD");

	guessTest.match("ABCD"); // Correct guess
	ASSERT_EQ(3, guessTest.remaining()); // Check that no guesses have been used
}

// Check no decrement on lockout guess
TEST(GuesserTest, remain_lockoutGuess){
	Guesser guessTest = Guesser("ABCD");
	
	guessTest.match("abcd"); // Lockout guess
	ASSERT_EQ(3, guessTest.remaining()); // Check that no guesses have been used
}


// Check decrement on incorrect guess w/o lockout
TEST(GuesserTest, remain_incorrrectGuess){
	Guesser guessTest = Guesser("ABCD");

	guessTest.match("ABC"); // Incorrect guess
	ASSERT_EQ(2, guessTest.remaining()); // Check that 1 guess has been used
	
}

// Check guesses cannot go below 0
TEST(GuesserTest, remain_limitGuess){
	Guesser guessTest = Guesser("ABCD");
	
	// Use all guesses
	guessTest.match("ABC"); 
	guessTest.match("ABC"); 
	guessTest.match("ABC"); 

	guessTest.match("ABC"); // Extra incorrect guess after no more guesses

	ASSERT_EQ(0, guessTest.remaining()); // Check guesses haven't been decremented past 0
}
