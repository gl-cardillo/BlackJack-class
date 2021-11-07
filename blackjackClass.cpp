#include <algorithm> // std::shuffle
#include <array>
#include <cassert>
#include <ctime> // std::time
#include <iostream>
#include <random> // std::mt19937

// Maximum score before losing.
const int maximumScore{ 21 };

// Minimum score that the dealer has to have.
const int minimumDealerScore{ 17 };

class Card{

public:

enum  Suits
{
    club,
    diamond,
    heart,
    spade,

    max_suits
};

enum  Ranks
{
    rank_2,
    rank_3,
    rank_4,
    rank_5,
    rank_6,
    rank_7,
    rank_8,
    rank_9,
    rank_10,
    rank_jack,
    rank_queen,
    rank_king,
    rank_ace,

    max_ranks
};

private:

    Ranks m_rank;
    Suits m_suit;
    

public:

    Card() {};
    Card(Ranks rank, Suits suit) : m_rank {rank} ,m_suit { suit} {};
    // display ranks and suits of card
    void  print() const {
    
        switch (m_rank) {

            case rank_2:      std::cout << '2';   break;
            case rank_3:      std::cout << '3';   break;
            case rank_4:      std::cout << '4';   break;
            case rank_5:      std::cout << '5';   break;
            case rank_6:      std::cout << '6';   break;
            case rank_7:      std::cout << '7';   break;
            case rank_8:      std::cout << '8';   break;
            case rank_9:      std::cout << '9';   break;
            case rank_10:     std::cout << 'T';   break;
            case rank_jack:   std::cout << 'J';   break;
            case rank_queen:  std::cout << 'Q';   break;
            case rank_king:   std::cout << 'K';   break;
            case rank_ace:    std::cout << 'A';   break;
            default:          std::cout << '?';   break;
    }
    
        switch (m_suit) {

            case club:       std::cout << 'C';   break;
            case diamond:    std::cout << 'D';   break;
            case heart:      std::cout << 'H';   break;
            case spade:      std::cout << 'S';   break;
            default:         std::cout << '?';   break;
    
    }
}
    //return value of the cards
    int value() const {

        switch (m_rank) {
        
            case rank_2:        return 2;
            case rank_3:        return 3;
            case rank_4:        return 4;
            case rank_5:        return 5;
            case rank_6:        return 6;
            case rank_7:        return 7;
            case rank_8:        return 8;
            case rank_9:        return 9;
            case rank_10:       return 10;
            case rank_jack:     return 10;
            case rank_queen:    return 10;
            case rank_king:     return 10;
            case rank_ace:      return 11;
            default: 
                assert(false && "should never happen");    return 0;
        }
    }
 
    
};

class Deck {

    std::array<Card,52>::size_type m_cardIndex {0};
    std::array<Card,52> m_deck;


public: 

    Deck() {

  //  loop to create deck.

    std::array<Card,52>::size_type index{ 0 };

    for (int suit{ 0 }; suit < (Card::Suits::max_suits); ++suit)
  {
        for (int rank{ 0 }; rank < (Card::Ranks::max_ranks); ++rank)
    {

        m_deck[index] = { static_cast<Card::Ranks>(rank), static_cast<Card::Suits>(suit) };
        ++index;
    }
  
  }

}
// print the deck
void print()
{   
    for (const  auto& cards : m_deck)
    { 
        cards.print();
        std::cout <<  ' ';
    }
    
    std::cout << '\n';
}

// shuffle the cards
void shuffle()
{
    static std::mt19937 mt{ static_cast<std::mt19937::result_type>(std::time(nullptr)) };

    std::shuffle(m_deck.begin(), m_deck.end(), mt);

    m_cardIndex = 0;
}
// grab card in order
const Card& dealCard() {

    assert(m_cardIndex < m_deck.size());
    return  m_deck[m_cardIndex++];
    
    
}

};

class Player {
    
    int m_score{0};

    public:
        //give card to the player
        int drawCard(Deck& deck)  {

            int value = deck.dealCard().value();
            m_score += value;
            
            return value;

        }
        //count value of the player's card
        int score() const {

            return m_score;
        }
        // if score is over 21 
        bool isBust() {

        return (m_score > maximumScore);
        }
};
//ask the player if wants another card
bool playerWantsHit()
{
    while (true)
    {
        std::cout << "(h) to hit, or (s) to stand: ";

        char ch{};
        std::cin >> ch;

        switch (ch)
        {
        case 'h':
            return true;
        case 's':
            return false;
        }
    }
}

// Returns true if the player went bust. False otherwise.
bool playerTurn( Deck& deck, Player& player)
{
    while (true)
    {
        if (player.isBust()) {
            std::cout << "You busted!\n";
            return true;
        } else
        {
            if (playerWantsHit())
            {
                int cardValue { player.drawCard(deck) };
                std::cout << "You were dealt a " << cardValue << " and now have " << player.score() << '\n';
            }
            else
            {
                // The player didn't go bust.
                return false;
            }
        }
    }
}

// Returns true if the dealer went bust. False otherwise.
bool dealerTurn( Deck& deck, Player& dealer)
{
    // Draw cards until we reach the minimum value.
    while (dealer.score() < minimumDealerScore)
    {
        int cardValue { dealer.drawCard(deck) };
        std::cout << "The dealer dealt a " << cardValue << " and now has " << dealer.score() << '\n';

    }

    // If the dealer's score is too high, they went bust.
    if (dealer.score() > maximumScore)
    {
        std::cout << "The dealer busted!\n";
        return true;
    }

    return false;
}

bool playBlackjack( Deck& deck)
{
    // Create the dealer and give them 1 card.
    
    Player dealer{};

    dealer.drawCard(deck) ;

    // The dealer's card is face up, the player can see it.
    std::cout << "The dealer is showing: " << dealer.score() << '\n';

    // Create the player and give them 2 cards.
    Player player{};
    player.drawCard(deck);
    player.drawCard(deck); 
    

    std::cout << "You have: " << player.score() << '\n';

    if (playerTurn(deck,  player))
    {
        // The player went bust.
        return false;
    }

    if (dealerTurn(deck,  dealer))
    {
        // The dealer went bust, the player wins.
        return true;
    }

    return (player.score() > dealer.score());
}


int main()
{
    Deck deck{};

    deck.shuffle();

    if (playBlackjack(deck)) {

        std::cout << "Player won the game\n";

    } else {

        std::cout << "Dealer won the game\n";
    }

    return 0;
}