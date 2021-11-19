clear

cd && cd Desktop/Cbot/src/com/company

gcc bot.c -o bot -pthread -ldiscord -lcurl -lcrypto -lm

./bot
