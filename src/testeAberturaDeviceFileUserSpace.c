#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

// TODO Macro para nome do dispositivo, passado no Makefile
// de modo a ter consistência entre os arquivos

// Programa de **user space** normal para abrir um
// arquivo comum a fim de testar o assignment do driver ao kernel
// TLDR: arquivo de dispositivo já deve estar criado, estamos apenas
// testando se abre.
// Ver: mknod (para criar device file), dmesg (p/ver os logs do kernel), chmod (para executar device file)

int main(void)
{

	int device = open("/dev/MaxonEposII_50/5");

	if (device == -1)
	{
		printf("Falha na abertura do dispositivo MaxonEposII_50/5\n");
		return -1;
	}

	printf("Dispositivo foi aberto com sucesso\n");
	close(device);
	return 0;
}
