# Scheduling com OpenMP

#### Cadeira: Programação Paralela - elc139
#### Nome: René Gargano Ferrari
#### Matrícula: 201812031

## Programa desenvolvido
[OpemMPDemoABC](https://github.com/elc139/t3-rgferrari/blob/master/OpenMPDemoABC.cpp)

## Exemplo de saída do programa
### Runtime
Quando o *schedule* está definido como runtime, ele irá utilizar a variável definida no sistema para o schedule. No caso abaixo foi escolhido o *dynamic*.
```
Case 1: runtime with mutex
ACBACBACBACBACBACBACBACBACBACBACBACBACBACBACBACBACBACBACBACB
A=20 B=20 C=20
```

### Static
Quando o *schedule* é definido para *static*, o OpenMP dividirá as iterações em chunks com tamanho definido pela variável chunk_size. As threads irão ser executadas em ordem cíclica. No caso do chunk_size não ser especificado, o OpenMP irá pegar o número de iterações total dividido pelo número de Threads e utilizar como chunk_size.
```
Case 2: static (with no chunk specification) with mutex
AAAAAAAAAAAAAAAAAAAABBBBBBBBBBBBBBBBBBBBCCCCCCCCCCCCCCCCCCCC
A=20 B=20 C=20

Case 3: static (with chunk specification) with mutex
AAAAAAAABBBBBBBBCCCCCCCCAAAAAAAABBBBBBBBCCCCCCCCAAAAAAAABBBB
A=24 B=20 C=16 
```

### Dynamic
Quando o *schedule* é definido para *dynamic*, o OpenMP dividirá as iterações em chunks com tamanho definido pela variável chunk_size. As threads serão executadas por ordem de chegada. A primeira thread que terminar de executar seu chunk irá entrar na fila para executar o próximo chunk. Caso o chunk_size não seja definido, ele será 1 por default.
```
Case 4: dynamic (with no chunk specification) with mutex
BACBACBACBACBACBACBACBACBACBACBACBACBACBACBACBACBACBACBACBAC
A=20 B=20 C=20

Case 5: dynamic (with chunk specification) with mutex
BBBBBBBBCCCCCCCCAAAAAAAABBBBBBBBCCCCCCCCAAAAAAAABBBBBBBBCCCC
A=16 B=24 C=20 
```

### Guided
Quando o *schedule* é definido para *guided*, o OpenMP dividirá as iterações em chunks equivalentes ao número de iterações ainda não executadas dividido pelo número de threads. Os chunks vão diminuindo de tamanho conforme a execução até o tamanho mínimo definido pela variável chunk_size. Caso a variável chunk_size não seja definida será utilizado o valor default 1. A ordem de execução dos chunks é igual ao *dynamic*.
```
Case 6: guided (with no chunk specification) with mutex
CCCCCCCCCCCCCCCCCCCCBBBBBBBBBBBBBBAAAAAAAAACCCCCCBBBBAAACCBA
A=13 B=19 C=28

Case 7: guided (with chunk specification) with mutex
BBBBBBBBBBBBBBBBBBBBAAAAAAAAAAAAAACCCCCCCCCBBBBBBBBAAAAAAAAC
A=22 B=28 C=10 
```

### Auto
Quando o *schedule* é definido para *auto* a decisão do *scheduling* é deixada para o compilador ou para o *runtime system*. No caso abaixo foi escolhida o *static*.
```
Case 8: auto with mutex
AAAAAAAAAAAAAAAAAAAABBBBBBBBBBBBBBBBBBBBCCCCCCCCCCCCCCCCCCCC
A=20 B=20 C=20 
```

### Casos sem mutex
Nos casos onde não é definida área crítica, algumas iterações são sobrescritas por outras de outras threads. Além disso, por causa da função *spendSomeTime* e da falta do segmento *#pragma omp ordered*, a ordem de execução das iterações não é mantida.
```
Case 9: static (with no chunk specification) (with no mutex)
CCBACBAABCABCBACBACBACBACBACBACBACABCABCABACBACBACBACBACB-C-
A=19 B=19 C=20

Case 10: static (with chunk specification) (with no mutex)
CCBACBACBACBABCABCABCA--CBACCABBCABCABACBACBCA--BABABABA-AAA
A=21 B=18 C=16 
```
## Referências
Jaka's Corner. OpenMP: For & Scheduling. http://jakascorner.com/blog/2016/06/omp-for-scheduling.html.
