#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include "DatosMemCompartida.h"

int main()
{
    DatosMemCompartida* pdatos;                       

    int fd = open("/tmp/datosBot", O_RDWR);          
    if (fd < 0) { perror("open"); return 1; }

    void* p = mmap(NULL, sizeof(DatosMemCompartida),
                   PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (p == MAP_FAILED) { perror("mmap"); close(fd); return 1; }

    close(fd);                                       
    pdatos = static_cast<DatosMemCompartida*>(p);     

    while (1)                                         
    {
        usleep(25000);
        float yRaqueta = (pdatos->raqueta1.y1 + pdatos->raqueta1.y2) / 2.0f;
        float yBola    = pdatos->esfera.centro.y;

        if (yBola > yRaqueta + 0.25f)      pdatos->accion = 1;   // subir
        else if (yBola < yRaqueta - 0.25f) pdatos->accion = -1;  // bajar
        else                               pdatos->accion = 0;   // nada
    }

    munmap(pdatos, sizeof(DatosMemCompartida));
    return 0;
}
