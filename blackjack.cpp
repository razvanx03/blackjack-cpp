#include <iostream>
#include <array>
#include <random>
#include <cassert>
#include <chrono>
#include <thread>


enum class Ranks {
	rank_2,
	rank_3,
	rank_4,
	rank_5,
	rank_6,
	rank_7,
	rank_8,
	rank_9,
	rank_10,
	Jack,
	Queen,
	King,
	Ace,

	totalRanks
};

enum class Suits {
	clubs,
	diamonds,
	hearts,
	spades,

	totalSuits
};

struct Card {
	Ranks rank{};
	Suits suit{};
};

std::string printCard(const Card& card) {

	std::string createdCard;

	switch (card.rank) {
	case Ranks::rank_2:  createdCard.push_back('2'); break;
	case Ranks::rank_3:  createdCard.push_back('3'); break;
	case Ranks::rank_4:  createdCard.push_back('4'); break;
	case Ranks::rank_5:  createdCard.push_back('5'); break;
	case Ranks::rank_6:  createdCard.push_back('6'); break;
	case Ranks::rank_7:  createdCard.push_back('7'); break;
	case Ranks::rank_8:  createdCard.push_back('8'); break;
	case Ranks::rank_9:  createdCard.push_back('9'); break;
	case Ranks::rank_10: createdCard.push_back('T'); break;
	case Ranks::Jack:	 createdCard.push_back('J'); break;
	case Ranks::Queen:	 createdCard.push_back('Q'); break;
	case Ranks::King:	 createdCard.push_back('K'); break;
	case Ranks::Ace:	 createdCard.push_back('A'); break;

	default: assert(false && "should never happen(1)");
	}
	switch (card.suit) {
	case Suits::clubs:	  createdCard.push_back('C'); break;
	case Suits::diamonds: createdCard.push_back('D'); break;
	case Suits::hearts:   createdCard.push_back('H'); break;
	case Suits::spades:   createdCard.push_back('S'); break;

	default: assert(false && "should never happen(2)");
	}
	return createdCard;
}

void printDeck(std::array<Card, 52>& deck) {

	for (auto& element : deck) {
		std::cout << printCard(element);
		std::cout << " ";
	}
}

std::array<Card, 52> createDeck() {

	std::array<Card, 52> card{};
	int index = 0;
	for (int suit = 0; suit < static_cast<int>(Suits::totalSuits); suit++)
	{
		for (int rank = 0; rank < static_cast<int>(Ranks::totalRanks); rank++)
		{
			card[index].suit = static_cast<Suits>(suit);
			card[index].rank = static_cast<Ranks>(rank);
			index++;
		}
	}

	return card;
}

void shuffleDeck(std::array<Card, 52>& Deck) {

	static std::mt19937 mt{ static_cast<std::mt19937::result_type>(std::time(nullptr)) };
	std::shuffle(Deck.begin(), Deck.end(), mt);
}

int getCardValue(const Card& card) {

	switch (card.rank) {
	case Ranks::rank_2: return 2;
	case Ranks::rank_3: return 3;
	case Ranks::rank_4: return 4;
	case Ranks::rank_5: return 5;
	case Ranks::rank_6: return 6;
	case Ranks::rank_7: return 7;
	case Ranks::rank_8: return 8;
	case Ranks::rank_9: return 9;
	case Ranks::rank_10:
	case Ranks::Jack:
	case Ranks::Queen:
	case Ranks::King:   return 10;
	case Ranks::Ace:    return 11;

	default: assert(false && "should never happen(3)");
	}
	return 0;
}

// false = dealer win
// true  = player win
bool playBlackjack(std::array<Card, 52> Deck) {

	shuffleDeck(Deck);
	/////////////////////
	//printDeck(Deck); 
	//std::cout << std::endl << std::endl;
	/////////////////////

	int dealer{ 0 };
	int player{ 0 };
	int hitOrStand{};
	int index{ 1 };

	dealer = dealer + getCardValue(Deck[index]);
	std::cout << "The dealer drew " << printCard(Deck[index]) << " and has \033[33m" << dealer << "\033[0m points.\n"
		<< "-------------------------------------\n";
	index++;

	player = player + getCardValue(Deck[index]);
	std::cout << "Your turn.\n\n" << "You drew " << printCard(Deck[index]);
	index++;
	player = player + getCardValue(Deck[index]);
	std::cout << " and " << printCard(Deck[index]) << " and have \033[33m" << player << "\033[0m points.\n\n";
	index++;
	if (player == 21)
		return true;
	std::cout << "Do you wish to hit or stand?\n";
	do
	{
		std::cout << "Press 1 to hit or 2 to stand: ";
		std::cin >> hitOrStand;

		if (hitOrStand == 1)
		{
			player = player + getCardValue(Deck[index]);
			std::cout << "\nYou drew " << printCard(Deck[index]) << " and now have \033[33m" << player << "\033[0m points.\n";
			index++;
			if (player == 21)
				return true;
		}
		else if (std::cin.fail() || hitOrStand > 2) {
			std::cout << "Invalid input." << std::endl;
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}
	} while (player < 21 && (hitOrStand == 1 || hitOrStand > 2 || hitOrStand == NULL));

	if (player > 21) {
		std::cout << "Your score is higher than 21.\n";
		return false;
	}

	std::cout << "-------------------------------------\n" << "Dealer's turn.\n\n";

	while (dealer < 17)
	{
		std::this_thread::sleep_for(std::chrono::duration<double>(2.5));
		dealer = dealer + getCardValue(Deck[index]);
		std::cout << "The dealer drew " << printCard(Deck[index]) << " and now has \033[33m" << dealer << "\033[0m points.\n";
		index++;
	}

	if (dealer == 21)
	{
		std::cout << "The dealer has reached a score of \033[33m 21\033[0m.\n";
		return false;
	}
	else if (dealer < 21)
	{
		std::cout << "The dealer chose to stay at \033[33m" << dealer << "\033[0m" << '\n'; // prost mode
		if (dealer >= player) {
			return false;
		}
		else return true;
	}
	else if (dealer > 21) {
		std::cout << "The dealer has reached a score higher than 21.\n";
		return true;
	}
	else return false;

	assert("should never happen(4)");
}

int main()
{
	std::array<Card, 52> Deck{ createDeck() };

	if (playBlackjack(Deck)) {
		std::cout << "You won :) ";
	}
	else std::cout << "You lost :( ";

	std::cout << std::endl << std::endl;

	return 0;
}