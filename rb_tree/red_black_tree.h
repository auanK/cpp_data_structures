#pragma once

#include <iostream>

#include "node.h"

// Classe que implementa uma árvore rubro-negra
template <typename type>
class red_black_tree {
   private:
    node<type> *_root;  // Ponteiro para a raiz da árvore
    node<type> *_nil;   // Nó sentinela que representa nulo (nil)

    // Rotação à esquerda em torno do nó x
    void left_rotate(node<type> *x) {
        node<type> *y = x->right;  // Salva o nó y (filho direito de x)

        // Redefine os filhos do nó x e do nó y
        x->right = y->left;
        if (y->left != _nil) {
            y->left->parent = x;  // Atualiza o pai do filho esquerdo de y
        }

        y->parent = x->parent;  // Atualiza o pai de y para o pai de x
        if (x->parent == _nil) {
            _root = y;  // Se x era a raiz, y se torna a nova raiz
        } else if (x == x->parent->left) {
            x->parent->left = y;  // Atualiza o filho esquerdo do pai de x
        } else {
            x->parent->right = y;  // Atualiza o filho direito do pai de x
        }

        // Finaliza a rotação
        y->left = x;
        x->parent = y;
    }

    // Rotação à direita em torno do nó x
    void right_rotate(node<type> *x) {
        node<type> *y = x->left;  // Salva o nó y (filho esquerdo de x)

        // Redefine os filhos do nó x e do nó y
        x->left = y->right;
        if (y->right != _nil) {
            y->right->parent = x;  // Atualiza o pai do filho direito de y
        }

        y->parent = x->parent;  // Atualiza o pai de y para o pai de x
        if (x->parent == _nil) {
            _root = y;  // Se x era a raiz, y se torna a nova raiz
        } else if (x == x->parent->left) {
            x->parent->left = y;  // Atualiza o filho esquerdo do pai de x
        } else {
            x->parent->right = y;  // Atualiza o filho direito do pai de x
        }

        // Finaliza a rotação
        y->right = x;
        x->parent = y;
    }

    // Encontra o nó com o menor valor na subárvore com raiz x
    node<type> *minimum(node<type> *x) {
        while (x->left != _nil) {
            x = x->left;  // Desce para o filho esquerdo
        }
        return x;  // x é o nó com o menor valor
    }

    // Encontra o nó com o maior valor na subárvore com raiz x
    node<type> *maximum(node<type> *x) {
        while (x->right != _nil) {
            x = x->right;  // Desce para o filho direito
        }
        return x;  // x é o nó com o maior valor
    }

    // Corrige a árvore após a inserção de um nó para manter as propriedades da
    // árvore rubro-negra
    void insert_fixup(node<type> *z) {
        while (z->parent->color == RED) {
            if (z->parent == z->parent->parent->left) {
                node<type> *y = z->parent->parent->right;  // Tio de z
                if (y->color == RED) {
                    // Caso 1: O tio de z é vermelho
                    z->parent->color = BLACK;
                    y->color = BLACK;
                    z->parent->parent->color = RED;
                    z = z->parent->parent;  // Move z para o avô
                } else {
                    if (z == z->parent->right) {
                        // Caso 2: z é o filho direito
                        z = z->parent;
                        left_rotate(z);  // Rotaciona à esquerda
                    }
                    // Caso 3: z é o filho esquerdo
                    z->parent->color = BLACK;
                    z->parent->parent->color = RED;
                    right_rotate(z->parent->parent);  // Rotaciona à direita
                }
            } else {
                node<type> *y = z->parent->parent->left;  // Tio de z
                if (y->color == RED) {
                    // Caso 1: O tio de z é vermelho
                    z->parent->color = BLACK;
                    y->color = BLACK;
                    z->parent->parent->color = RED;
                    z = z->parent->parent;  // Move z para o avô
                } else {
                    if (z == z->parent->left) {
                        // Caso 2: z é o filho esquerdo
                        z = z->parent;
                        right_rotate(z);  // Rotaciona à direita
                    }
                    // Caso 3: z é o filho direito
                    z->parent->color = BLACK;
                    z->parent->parent->color = RED;
                    left_rotate(z->parent->parent);  // Rotaciona à esquerda
                }
            }
        }
        _root->color = BLACK;  // Garante que a raiz seja preta
    }

    // Remove o nó z da árvore e ajusta a árvore para manter suas propriedades
    void remove(node<type> *z) {
        node<type> *y, *x;
        if (z->left == _nil || z->right == _nil) {
            y = z;  // Caso em que z tem no máximo um filho
        } else {
            y = minimum(z->right);  // Caso em que z tem dois filhos
        }

        // Define x como o filho de y
        if (y->left != _nil) {
            x = y->left;
        } else {
            x = y->right;
        }

        // Atualiza o pai de x
        x->parent = y->parent;
        if (y->parent == _nil) {
            _root = x;  // Se y era a raiz, x se torna a nova raiz
        } else if (y == y->parent->left) {
            y->parent->left = x;  // Atualiza o filho esquerdo do pai de y
        } else {
            y->parent->right = x;  // Atualiza o filho direito do pai de y
        }

        // Copia o valor de y para z se necessário
        if (y != z) {
            z->key = y->key;
        }

        if (y->color == BLACK) {
            remove_fixup(x);  // Corrige a árvore se necessário
        }

        delete y;  // Libera a memória do nó removido
    }

    // Corrige a árvore após a remoção de um nó para manter as propriedades da
    // árvore rubro-negra
    void remove_fixup(node<type> *x) {
        while (x != _root && x->color == BLACK) {
            if (x == x->parent->left) {
                node<type> *w = x->parent->right;  // Irmão de x
                if (w->color == RED) {
                    // Caso 1: O irmão de x é vermelho
                    w->color = BLACK;
                    x->parent->color = RED;
                    left_rotate(x->parent);  // Rotaciona à esquerda
                    w = x->parent->right;    // Atualiza w
                }
                if (w->left->color == BLACK && w->right->color == BLACK) {
                    // Caso 2: Ambos os filhos de w são pretos
                    w->color = RED;
                    x = x->parent;  // Move x para o pai
                } else {
                    if (w->right->color == BLACK) {
                        // Caso 3: O filho direito de w é preto
                        w->left->color = BLACK;
                        w->color = RED;
                        right_rotate(w);       // Rotaciona à direita
                        w = x->parent->right;  // Atualiza w
                    }
                    // Caso 4: O filho direito de w é vermelho
                    w->color = x->parent->color;
                    x->parent->color = BLACK;
                    w->right->color = BLACK;
                    left_rotate(x->parent);  // Rotaciona à esquerda
                    x = _root;               // Encerra o loop
                }
            } else {
                node<type> *w = x->parent->left;  // Irmão de x
                if (w->color == RED) {
                    // Caso 1: O irmão de x é vermelho
                    w->color = BLACK;
                    x->parent->color = RED;
                    right_rotate(x->parent);  // Rotaciona à direita
                    w = x->parent->left;      // Atualiza w
                }
                if (w->right->color == BLACK && w->left->color == BLACK) {
                    // Caso 2: Ambos os filhos de w são pretos
                    w->color = RED;
                    x = x->parent;  // Move x para o pai
                } else {
                    if (w->left->color == BLACK) {
                        // Caso 3: O filho esquerdo de w é preto
                        w->right->color = BLACK;
                        w->color = RED;
                        left_rotate(w);       // Rotaciona à esquerda
                        w = x->parent->left;  // Atualiza w
                    }
                    // Caso 4: O filho esquerdo de w é vermelho
                    w->color = x->parent->color;
                    x->parent->color = BLACK;
                    w->left->color = BLACK;
                    right_rotate(x->parent);  // Rotaciona à direita
                    x = _root;                // Encerra o loop
                }
            }
        }
        x->color = BLACK;  // Garante que x seja preto
    }

    // Função auxiliar que imprime a árvore de forma visual
    void bshow(node<type> *n, std::string heranca) {
        if (n != _nil && (n->left != _nil || n->right != _nil)) {
            bshow(n->right, heranca + "r");  // Imprime o filho direito
        }

        // Imprime a linha de herança
        for (int i = 0; i < (int)heranca.size() - 1; i++) {
            std::cout << (heranca[i] != heranca[i + 1] ? "│   " : "    ");
        }

        // Imprime o marcador de conexão com o pai
        if (heranca != "") {
            std::cout << (heranca.back() == 'r' ? "┌───" : "└───");
        }

        // Imprime o nó atual
        if (n == _nil) {
            std::cout << "#" << std::endl;  // Nó nulo
            return;
        }

        std::cout << n->key << (n->color == RED ? "R" : "B") << std::endl;

        if (n != _nil && (n->left != _nil || n->right != _nil)) {
            bshow(n->left, heranca + "l");  // Imprime o filho esquerdo
        }
    }

   public:
    // Construtor que inicializa a árvore com um nó sentinela _nil
    red_black_tree() {
        _root = _nil = new node<type>(0, BLACK, nullptr, nullptr, nullptr);
        _nil->left = _nil->right = _nil->parent = _nil;
    }

    // Insere um novo valor na árvore
    void insert(type value) {
        node<type> *current = _root;
        node<type> *current_father = _nil;

        // Encontra o local correto para inserir o novo nó
        while (current != _nil) {
            current_father = current;
            if (value < current->key) {
                current = current->left;
            } else if (value > current->key) {
                current = current->right;
            } else {
                return;  // O valor já existe, não insere
            }
        }

        // Cria o novo nó e define seu pai
        node<type> *new_node =
            new node<type>(value, RED, _nil, _nil, current_father);
        if (current_father == _nil) {
            _root = new_node;  // A árvore estava vazia
        } else if (value < current_father->key) {
            current_father->left = new_node;  // Insere como filho esquerdo
        } else {
            current_father->right = new_node;  // Insere como filho direito
        }

        // Corrige a árvore para manter as propriedades da árvore rubro-negra
        insert_fixup(new_node);
    }

    // Remove um valor da árvore
    void remove(type value) {
        node<type> *p = _root;
        while (p != _nil && p->key != value) {
            if (value < p->key) {
                p = p->left;
            } else {
                p = p->right;
            }
        }

        if (p != _nil) {
            remove(p);  // Remove o nó encontrado
        }
    }

    // Encontra o menor valor na árvore
    type minimum() { return minimum(_root)->key; }

    // Encontra o maior valor na árvore
    type maximum() { return maximum(_root)->key; }

    // Imprime a árvore de forma visual
    void print() { bshow(_root, ""); }
};
