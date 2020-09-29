# DES (Rede-de-Feistel)

O intuito desse programa é fazer a encriptação em bloco de mensagens de texto utilizando o algoritmo DES ([Data Encription Standart](https://pt.wikipedia.org/wiki/Data_Encryption_Standard)) com o modo de funcionamento ECB ([Eletronic Code Book](https://pt.wikipedia.org/wiki/Modo_de_opera%C3%A7%C3%A3o_(criptografia)#Modo_ECB_(Electronic_CodeBook))). Com a possibilidade de visualizar, passo a passo, os bits dos resultados.

O usuário entra com arquivos contendo a mensagem e a chave de encriptação e o programa mostra a geração de subchaves, rodadas da Rede de Feistel e encriptação completa dos blocos, gerando um arquivo que contem a cifra. Em caso de decriptação o comportamento é o mesmo, com o usuário fornecendo a cifra e a chave.

Algoritmo foi desenvolvido como parte da disciplina de Criptografia para Segurança de Dados do curso de Bacharelado em Sistemas de Informação pela Universidade Federal de Santa Maria - UFSM. Pode ser usado como ferramenta para mostrar o comportamento do DES e as mudanças bit a bit.

## O algoritmo
Ele usa blocos de 8 bytes, tanto para a chave quanto para o plaintext. Se a chave for menor que o bloco, é realizado padding com 0s, se for maior, usa-se apenas os 8 primeiros bytes.

Realiza 16 rodadas na Rede de Feistel para garantir uma boa difusão e confusão. Com a função F realizando um XOR entre chave e a parte recebida, após realiza a difusão seguindo as tabelas de [SBOX](https://upload.wikimedia.org/wikipedia/commons/4/44/DES_S-box.jpg) e uma permutação.

As subchaves de rodada são geradas com shifts circulares nas duas subpartes da subchave que foi gerada com uma permutação.


## Modo de uso

Em ambos os modos o usuário pode escolher se as saídas devem ser mostradas, nesse caso, recomenda-se usar um texto claro (plaintext) pequeno para que seja melhor de visualizar e compreender as mensagens, pois plaintext grandes gerarão muitas mensagens de saída.

##### Encriptação
Para a encriptação o usuário deve:

   1. Fornecer um arquivo com extensão .txt contendo o plaintext a ser encriptado;
   2. Definir o nome do arquivo em que a cifra será gravada;
   3. Fornecer um arquivo com extensão .txt contendo a chave a ser usada para a encriptação;
   4. Após o processo terminar uma mensagens de sucesso será exibida e o texto cifrado estará no arquivo com o nome fornecido.


##### Decriptação
Para a decriptação o usuário deve:

   1. Fornecer um arquivo com extensão .txt contendo a cifra a ser decriptada;
   2. Definir o nome do arquivo em que o plaintext será gravado;
   3. Fornecer um arquivo com extensão .txt contendo a chave a ser usada para a encriptação;
   4. Após o processo terminar uma mensagens de sucesso será exibida e o texto decifrado estará no arquivo com o nome fornecido.

Para os nomes de arquivos, digitar apenas o nome do arquivo. Se o arquivo para a cifra já existir, seu conteúdo será apagado. Se o arquivo para o plaintext já existir, seu conteúdo será apagado.
Um exemplo de plaintext pode ser visualizado em [exemplo_plaintext.txt](https://github.com/ddvargas/Rede-de-Feistel/blob/master/exemplo_plaintext.txt).
Um exemplo de chave pode ser visto em [exemplo_chave.txt](https://github.com/ddvargas/Rede-de-Feistel/blob/master/exemplo_chave.txt).


### Compilação
Usar o CMake e ou o GCC. 


