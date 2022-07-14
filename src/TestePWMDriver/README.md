		Diego Fleury Correa de Moraes
        Nusp: 11800584

        Este driver simples de PWM para uma raspberry pi 3 B+ tem como 
        funcao a regulacao da frequencia de uma saida pwm do dispositivo.

        Para que a execucao seja possivel, um modulo de kernel que faz o mapeamento
        do espaco de enderecamento para as registradoras do controlador do pwm eh 
        necessario. Para ver qual esta (se está) disponivel em sua pi, ver atraves 
        do comando 'lsmod | grep pwm' (essencialmente listar os modulos e ver aqueles com o
        nome 'pwm' em alguma parte). Como estou no Ubuntu Server 20.04 o modulo nao esta
        nativamente instalado, logo devemos instalar um novo. O utilizado para testes fora 
        o popular pwm_bcm2835, usado para controlar interface pwm nos chips da Qualcom (processador da pi).

        Para usar, compile o programa (make na pasta em que está localizado),
        insira o modulo no kernel (sudo insmod driverPWM.ko), mude as permissoes
        do device file (permitir ler, escrever e executar, sudo chmod 666 /dev/PWM_DRIVER).

        Para mandar um comando ao pwm, fazer no terminal:

        echo INTENSIDADE > /dev/PWM_DRIVER

        em que INTENSIDADE eh um valor entre '0' e '9'.

        Para remover do kernel o modulo, escrever no shell 'sudo rmmod PWM_DRIVER'.
