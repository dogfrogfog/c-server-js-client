#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 4096
#define MAX_ITEMS 100
#define MAX_TYPES 2
#define MAX_ABILITIES 3
#define MAX_RESPONSE_SIZE 8192

// Updated structure to match Pokemon data
struct Stats {
    int hp;
    int attack;
    int defense;
    int speed;
};

struct Pokemon {
    int id;
    char name[50];
    char type[MAX_TYPES][20];
    int type_count;
    struct Stats stats;
    char abilities[MAX_ABILITIES][30];
    int ability_count;
    char image[200];
};

// Global variables
struct Pokemon pokemons[MAX_ITEMS];
int pokemon_count = 0;

// Initialize Pokemon data
void init_pokemon_data() {
    // Pokemon 1: Pikachu
    pokemons[0].id = 1;
    strcpy(pokemons[0].name, "Pikachu");
    strcpy(pokemons[0].type[0], "Electric");
    pokemons[0].type_count = 1;
    pokemons[0].stats.hp = 35;
    pokemons[0].stats.attack = 55;
    pokemons[0].stats.defense = 40;
    pokemons[0].stats.speed = 90;
    strcpy(pokemons[0].abilities[0], "Static");
    strcpy(pokemons[0].abilities[1], "Lightning Rod");
    pokemons[0].ability_count = 2;
    strcpy(pokemons[0].image, "https://raw.githubusercontent.com/PokeAPI/sprites/master/sprites/pokemon/25.png");

    // Pokemon 2: Charizard
    pokemons[1].id = 2;
    strcpy(pokemons[1].name, "Charizard");
    strcpy(pokemons[1].type[0], "Fire");
    strcpy(pokemons[1].type[1], "Flying");
    pokemons[1].type_count = 2;
    pokemons[1].stats.hp = 78;
    pokemons[1].stats.attack = 84;
    pokemons[1].stats.defense = 78;
    pokemons[1].stats.speed = 100;
    strcpy(pokemons[1].abilities[0], "Blaze");
    strcpy(pokemons[1].abilities[1], "Solar Power");
    pokemons[1].ability_count = 2;
    strcpy(pokemons[1].image, "https://raw.githubusercontent.com/PokeAPI/sprites/master/sprites/pokemon/6.png");

    // Pokemon 3: Bulbasaur
    pokemons[2].id = 3;
    strcpy(pokemons[2].name, "Bulbasaur");
    strcpy(pokemons[2].type[0], "Grass");
    strcpy(pokemons[2].type[1], "Poison");
    pokemons[2].type_count = 2;
    pokemons[2].stats.hp = 45;
    pokemons[2].stats.attack = 49;
    pokemons[2].stats.defense = 49;
    pokemons[2].stats.speed = 45;
    strcpy(pokemons[2].abilities[0], "Overgrow");
    strcpy(pokemons[2].abilities[1], "Chlorophyll");
    pokemons[2].ability_count = 2;
    strcpy(pokemons[2].image, "https://raw.githubusercontent.com/PokeAPI/sprites/master/sprites/pokemon/1.png");

    // Pokemon 4: Squirtle
    pokemons[3].id = 4;
    strcpy(pokemons[3].name, "Squirtle");
    strcpy(pokemons[3].type[0], "Water");
    pokemons[3].type_count = 1;
    pokemons[3].stats.hp = 44;
    pokemons[3].stats.attack = 48;
    pokemons[3].stats.defense = 65;
    pokemons[3].stats.speed = 43;
    strcpy(pokemons[3].abilities[0], "Torrent");
    strcpy(pokemons[3].abilities[1], "Rain Dish");
    pokemons[3].ability_count = 2;
    strcpy(pokemons[3].image, "https://raw.githubusercontent.com/PokeAPI/sprites/master/sprites/pokemon/7.png");

    // Pokemon 5: Jigglypuff
    pokemons[4].id = 5;
    strcpy(pokemons[4].name, "Jigglypuff");
    strcpy(pokemons[4].type[0], "Normal");
    strcpy(pokemons[4].type[1], "Fairy");
    pokemons[4].type_count = 2;
    pokemons[4].stats.hp = 115;
    pokemons[4].stats.attack = 45;
    pokemons[4].stats.defense = 20;
    pokemons[4].stats.speed = 20;
    strcpy(pokemons[4].abilities[0], "Cute Charm");
    strcpy(pokemons[4].abilities[1], "Competitive");
    strcpy(pokemons[4].abilities[2], "Friend Guard");
    pokemons[4].ability_count = 3;
    strcpy(pokemons[4].image, "https://raw.githubusercontent.com/PokeAPI/sprites/master/sprites/pokemon/39.png");

    pokemon_count = 5;
    printf("Initialized %d Pokemon\n", pokemon_count);
}

// Function to parse HTTP request
void parse_request(char *request, char *method, char *path) {
    sscanf(request, "%s %s", method, path);
}

// Function to send HTTP response
void send_response(int client_socket, char *status, char *content_type, char *body) {
    char response[MAX_RESPONSE_SIZE];
    if (strlen(body) > MAX_RESPONSE_SIZE - 200) { // Leave room for headers
        fprintf(stderr, "Response body too large\n");
        return;
    }
    
    int written = snprintf(response, MAX_RESPONSE_SIZE,
            "HTTP/1.1 %s\r\n"
            "Content-Type: %s\r\n"
            "Content-Length: %lu\r\n"
            "Connection: close\r\n"
            "\r\n"
            "%s",
            status,
            content_type,
            strlen(body),
            body);
            
    if (written >= MAX_RESPONSE_SIZE) {
        fprintf(stderr, "Response buffer overflow prevented\n");
        return;
    }
    
    send(client_socket, response, strlen(response), 0);
}

// Updated handle_get_all route
void handle_get_all(int client_socket) {
    char *json = malloc(MAX_RESPONSE_SIZE);
    char pokemon_json[1024];
    
    if (!json) {
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }
    
    strcpy(json, "[");
    size_t current_length = 1;
    
    for (int i = 0; i < pokemon_count; i++) {
        // Create JSON string for types
        char types_json[100] = "[";
        for (int j = 0; j < pokemons[i].type_count; j++) {
            char type_str[50];
            snprintf(type_str, sizeof(type_str), 
                    "%s\"%s\"", 
                    j == 0 ? "" : ",", 
                    pokemons[i].type[j]);
            strcat(types_json, type_str);
        }
        strcat(types_json, "]");

        // Create JSON string for abilities
        char abilities_json[150] = "[";
        for (int j = 0; j < pokemons[i].ability_count; j++) {
            char ability_str[50];
            snprintf(ability_str, sizeof(ability_str), 
                    "%s\"%s\"", 
                    j == 0 ? "" : ",", 
                    pokemons[i].abilities[j]);
            strcat(abilities_json, ability_str);
        }
        strcat(abilities_json, "]");

        int written = snprintf(pokemon_json, sizeof(pokemon_json),
                "%s{\"id\":%d,\"name\":\"%s\",\"type\":%s,\"stats\":{\"hp\":%d,\"attack\":%d,\"defense\":%d,\"speed\":%d},\"abilities\":%s,\"image\":\"%s\"}",
                i == 0 ? "" : ",",
                pokemons[i].id,
                pokemons[i].name,
                types_json,
                pokemons[i].stats.hp,
                pokemons[i].stats.attack,
                pokemons[i].stats.defense,
                pokemons[i].stats.speed,
                abilities_json,
                pokemons[i].image);

        if (current_length + written >= MAX_RESPONSE_SIZE - 2) {
            fprintf(stderr, "Buffer would overflow, truncating response\n");
            break;
        }

        strcat(json, pokemon_json);
        current_length += written;
    }
    
    strcat(json, "]");
    send_response(client_socket, "200 OK", "application/json", json);
    free(json);
}

// Updated handle_get_item route
void handle_get_item(int client_socket, char *path) {
    int id;
    sscanf(path, "/item/%d", &id);
    
    for (int i = 0; i < pokemon_count; i++) {
        if (pokemons[i].id == id) {
            char json[BUFFER_SIZE];
            
            // Create JSON string for types
            char types_json[100] = "[";
            for (int j = 0; j < pokemons[i].type_count; j++) {
                char type_str[50];
                sprintf(type_str, "%s\"%s\"", j == 0 ? "" : ",", pokemons[i].type[j]);
                strcat(types_json, type_str);
            }
            strcat(types_json, "]");

            // Create JSON string for abilities
            char abilities_json[150] = "[";
            for (int j = 0; j < pokemons[i].ability_count; j++) {
                char ability_str[50];
                sprintf(ability_str, "%s\"%s\"", j == 0 ? "" : ",", pokemons[i].abilities[j]);
                strcat(abilities_json, ability_str);
            }
            strcat(abilities_json, "]");

            sprintf(json, 
                    "{\"id\":%d,\"name\":\"%s\",\"type\":%s,\"stats\":{\"hp\":%d,\"attack\":%d,\"defense\":%d,\"speed\":%d},\"abilities\":%s,\"image\":\"%s\"}",
                    pokemons[i].id,
                    pokemons[i].name,
                    types_json,
                    pokemons[i].stats.hp,
                    pokemons[i].stats.attack,
                    pokemons[i].stats.defense,
                    pokemons[i].stats.speed,
                    abilities_json,
                    pokemons[i].image);
            
            send_response(client_socket, "200 OK", "application/json", json);
            return;
        }
    }
    
    send_response(client_socket, "404 Not Found", "application/json", "{\"error\": \"Pokemon not found\"}");
}

int main() {
    // Replace load_pokemon_data() with init_pokemon_data()
    init_pokemon_data();
    
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE];
    
    // Create socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("Socket creation failed");
        exit(1);
    }
    
    // Add this section - Enable address reuse
    int opt = 1;
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt failed");
        exit(1);
    }
    
    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);
    
    // Bind socket
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Socket binding failed");
        exit(1);
    }
    
    // Listen for connections
    if (listen(server_socket, 10) < 0) {
        perror("Listen failed");
        exit(1);
    }
    
    printf("Server listening on port %d...\n", PORT);
    
    while (1) {
        // Accept client connection
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_len);
        if (client_socket < 0) {
            perror("Accept failed");
            continue;
        }
        
        // Read request with error checking
        memset(buffer, 0, BUFFER_SIZE);
        ssize_t bytes_received = recv(client_socket, buffer, BUFFER_SIZE - 1, 0);
        if (bytes_received < 0) {
            perror("Receive failed");
            close(client_socket);
            continue;
        }
        buffer[bytes_received] = '\0';
        
        printf("Received request: %s\n", buffer); // Debug print
        
        // Parse request
        char method[10], path[100];
        parse_request(buffer, method, path);
        
        printf("Parsed method: %s, path: %s\n", method, path); // Debug print
        
        // Route handling
        if (strcmp(method, "GET") == 0) {
            if (strcmp(path, "/all") == 0) {
                handle_get_all(client_socket);
            } else if (strncmp(path, "/item/", 6) == 0) {
                handle_get_item(client_socket, path);
            } else {
                send_response(client_socket, "404 Not Found", "application/json", 
                            "{\"error\": \"Route not found\"}");
            }
        } else {
            send_response(client_socket, "405 Method Not Allowed", "application/json", 
                         "{\"error\": \"Method not allowed\"}");
        }
        
        // Close client socket
        close(client_socket);
    }
    
    // Close server socket
    close(server_socket);
    return 0;
}
