
# Space Invaders

## Windows

#### Requisitos:

GCC

#### Instalação:

```Powershell
choco install mingw
```


#### Compilando o codigo:

```Powershell
gcc .\main.c -o SpaceInvaders -L .\lib\ -lSOIL -lopengl32 -lgdi32
```

#### Executando:

```Powershell
.\SpaceInvaders.exe
```

obs: Só funciona no windows