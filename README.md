### README.md

#### English: 🇬🇧 🇺🇸

# Library Management System

This project is a simple library management system implemented in C. It includes a server that manages a collection of books and serves client requests. Clients can query the available books, reserve a book, or return a book.

## Features

- **Listing Available Books**: Clients can request a list of available books in the library.
  
- **Reserving a Book**: Clients can reserve a book if it's available.

- **Returning a Book**: Clients can return a previously reserved book.

## Usage

1. **Compile the Code:**
   ```bash
   gcc biblioteca.c -o biblioteca
   ```

2. **Run the Server:**
   ```bash
   ./biblioteca 6
   ```
   Replace `6` with the desired number of books.

3. **Connect Clients:**
   Use a client program to connect to the server and perform actions.

   Example:
   ```bash
   ./client L 0   # List available books
   ./client P 2   # Reserve book with ID 2
   ./client R 1   # Return book with ID 1
   ```

## Contributing

Feel free to contribute to this project by submitting bug reports or feature requests.

## License

This project is licensed under the MIT.

---

#### Italiano: 🇮🇹

# Sistema di Gestione Bibliotecaria

Questo progetto è un semplice sistema di gestione bibliotecaria implementato in C. Include un server che gestisce una collezione di libri e serve le richieste dei client. I client possono richiedere l'elenco dei libri disponibili, prenotare un libro o restituire un libro.

## Funzionalità

- **Elenca Libri Disponibili**: I client possono richiedere un elenco dei libri disponibili nella biblioteca.
  
- **Prenota un Libro**: I client possono prenotare un libro se è disponibile.

- **Restituisci un Libro**: I client possono restituire un libro precedentemente prenotato.

## Utilizzo

1. **Compila il Codice:**
   ```bash
   gcc biblioteca.c -o biblioteca
   ```

2. **Avvia il Server:**
   ```bash
   ./biblioteca 6
   ```
   Sostituisci `6` con il numero desiderato di libri.

3. **Connetti i Client:**
   Usa un programma client per connetterti al server e eseguire azioni.

   Esempio:
   ```bash
   ./client L 0   # Elenca i libri disponibili
   ./client P 2   # Prenota il libro con ID 2
   ./client R 1   # Restituisci il libro con ID 1
   ```

## Contributi

Sentiti libero di contribuire a questo progetto inviando segnalazioni di bug o richieste di nuove funzionalità.

## Licenza

Questo progetto è concesso in licenza secondo i termini della MIT.

--- 
