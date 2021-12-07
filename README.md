# T1 - Jogo da Forca 

## Alunos 
  André Filipe da Silva Fernandes - 19100515
  José Luis de Souza - 

## Detalhes de Implementação
  Na implementação deste projeto usamos 3 arquivos: server.c, responsável por gerenciar o servidor; client.c, responsável por gerenciar a comunicação do cliente; e hangman.c, que contêm as funções e estruturas necessárias para o funcionamento do jogo.

  O servidor usa a chamada poll para gerenciar a conexão entre os clientes. A cada novo cliente que conecta, o servidor cria uma nova struct Hangman, que é inicializada com uma palavra aleatória da lista de palavras. 

  A cada caractere enviado pelo usuário, o servidor valida a entrada com a função evaluate em hangman.c, atualiza o estado da estrutura, e retorna o status da validação que pode ser CORRECT, WRONG, WIN ou LOSE. Depois de validada a entrada, o servidor envia ao cliente informações importantes, como os espaços ainda vagos e os que já foram preenchidos na palavra, a quantidade de erros que já ocorreram (que é utilizada para descobrir qual parte do corpo deve ser desenhada na forca), e o status da validação.

  No lado do cliente a função main realiza a conexão com o servidor e em seguida inicia o game loop. Dentro do game loop acontecerá toda a interação com o usuário e a comunicação com o servidor. 