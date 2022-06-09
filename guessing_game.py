import random
import time
print("Welcome! to the new guessing game you could select in between 1 to 100")
time.sleep(4)
print('Your game is processing.... ')
time.sleep(2)
user_guess= int(input('Guess your number: '))
correct_value= random.randint(1,100)
guess_count = 1
while user_guess != correct_value:
    time.sleep(1)
    guess_count += 1
    
    if user_guess < correct_value:
        user_guess =int(input('Wrong! your selection is lower than desired value. Guess again\n Your guess: '))
    elif user_guess > correct_value:
        user_guess = int(input('your selection is higher than desired value. Guess again\n Your guess: '))
print(f"good job! you selected the correct guess which is {correct_value}. But it took you {guess_count} number of guesses")

