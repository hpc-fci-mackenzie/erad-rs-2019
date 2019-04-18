# Paralelismo de tarefas utilizando OpenMP 4.5
Este respositório foi criado com o intúiro de facilitar o desenvolvimento do trabalho prático aos alunos do minucurso *Paralelismo de tarefas utilizando OpenMP 4.5* da *XIX Escola Regional de Alto Desemepenho da Região Sul*.

Os exemplos apresentados neste minicurso estão desenvolvidos na linguagem C com a utilização de diretivas de compilação do padrão *OpenMP 4.5* para implementação da estratégia de paralelismo à nível de tarefas. Desta forma, é necessário que o aluno faça a instalação de alguns pacotes. É importante também resssaltar que estamos considerando que o aluno utiliza um sistema operacional Linux e, dependendo da distribuição, os comandos aqui apresentados podem sofrer alguma alteração.

Este repositório está disponível sob a licença XXX. Portanto, todos têm acesso irrestrito.

## Como utilizar este repositório

### Pré-requisito
Para que o aluno consiga utilizar o conteúdo deste repositório, é necessário a instalação dos seguintes pacotes:
* git 2+;
* gcc-7+;
* OpenMP 4.5+;
* make 4+;
* vim 8+.

### Clone
Após a instalação dos pacotes necessários, o aluno deve *clonar* este repositório em sua máquina. Para que isso seja feito, o comando *git clone https://github.com/hpc-fci-mackenzie/erad-rs-2019.git* deve ser executado dentro do diretório de trabalho definido.

## Executando os exemplos
Há 4 exemplos para este minucurso. O Exemplo 1 é a implementação do algoritmo *quicksort*, o exemplo 2 é a implementação do algoritmo ACO e o exemplo 3 é a implementação do algoritmo ACO. O exemplo 4 é a implementação do algoritmo de solução de sistemas de equações usando o método de elimininação de Gauss para aplicação de técnicas de dependência de tarefas. O detalhamento dos algoritmos está disponível no material de apoio que também está contido neste repositório.

Em cada um dos diretórios de exemplo existe um arquivo nomeado como Makefile. Este arquivo contém toda a definição da compilação do exemplo, bastando apenas a execução do comando *make* dentro do diretório. Se houver algum erro, verifique se a etapa de pré-requisitos está completa.
