# Atelier Backend

## Setup
- Update db information in `main.cpp:27`
- Change listener in `main.cpp:31`

## Getting Started
To compile and start the project, use the following command:
```bash
g++ main.cpp endpoints/*/*/*.cpp -o main -lboost_system -lcpprest -lssl -lcrypto -I/usr/include -lpq
```