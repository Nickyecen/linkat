# Linkat

O propósito deste programa é criar uma maneira simples de adicionar includes/links a linguagens ou mesmo arquivos de texto que não possuem essa propriedade. O nome do programa vem de "link", por fazer algo similar ao link de bibliotecas, com "cat", o programa linux que concatena textos.

## Uso:
`$ linkat [-r] filename.ext expected_header expected_tail`

## Funcionamento
O programa irá abrir o arquivo `filename.ext`, parseá-lo em estruturas chamadas INodes (Inclusion Nodes), linká-lo com os arquivos requisitados e imprimir o resultado da linkagem em stdout. O parsing é feito com a busca do `expected_header`. Ao encontrar o `expected_header`, o programa irá ler toda a sequência até encontrar `expected_tail` e tratar isso como o nome de um arquivo. As estruturas fora de `expected_header` e `expected_tail` são chamados Blocos, as dentro, Links. Ao terminar de parsear o arquivo cujo nome foi encontrado, o programa retorna ao arquivo original e emite os caracteres após o tail. O `expected_header` e `expected_tail` são removidos do texto final, mantendo apenas os blocos e texto - ou blocos, se a flag de recursão estiver ativa - dos arquivos linkados.

### Ex.:
- arquivo1.txt:
```
world
```

- arquivo2.txt:
```
hello, --[[arquivo1.txt]]!
```

- Execucao:
`$ linkat arquivo2.txt "--\[\[" "]]"`

- stdout:
```
hello, world!
```

## Flags
- `-r`: realiza a busca recursivamente, incluindo includes de mesmo header e tail nos arquivos que forem encontrados dentro do arquivo chamado.
