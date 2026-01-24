# Linkat

O proposito desse programa e criar uma maneira simples de adicionar includes a linguagens ou mesmo arquivos de texto que nao possuem essa propriedade. O nome do programa vem de "link", por fazer algo similar ao link de bibliotecas, com "cat", o programa linux que concatena textos.

## Uso:
`$ linkat [-r] filename.ext expected_header expected_tail`

## Funcionamento
O programa ira abrir o arquivo `filename.ext`, le-lo caractere por caractere e sempre e imprimir esse caractere em stdout ate encontrar a sequencia `expected_header`. Ao encontrar o `expected_header`, o programa ira ler toda a sequencia ate encontrar `expected_tail` e tratar isso como o nome de um arquivo. Entao, se abrira esse arquivo e se emitira os caracteres dele. Ao terminar o arquivo, o programa retorna ao arquivo original e emite os caracteres apos o tail.

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
