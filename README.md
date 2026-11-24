# xv6riscv-process-suspend-resume

Suspensión y reanudación de procesos por PID en xv6-riscv.

## Avance

1. **Estado `SUSPENDED`**: nuevo estado en `enum procstate`; el planificador no ejecuta procesos suspendidos.
2. **Suspensión (`ksuspend`)**: marca el proceso (`suspend_req`) y este pasa a `SUSPENDED` en un punto seguro (`suspend_check()`, al volver a modo usuario, sin locks del kernel). No se puede suspender a `init`.
3. **Reanudación (`kresume`)**: el proceso vuelve a `RUNNABLE`, o se cancela la suspensión pendiente. Llamadas al sistema `suspend(pid)` y `resume(pid)`, y los comandos del shell `suspend` y `resume`.
4. **Consulta y pruebas**: llamada `pstate(pid)` que devuelve el estado del proceso, comando `pstate` y programa `suspendtest` con 8 pruebas automáticas.

## Uso

```
make qemu
$ suspend <pid>
$ resume <pid>
$ pstate [pid]
$ suspendtest
```

Retornan `0` si la operación es correcta, o un código negativo `SUSP_*` (ver `kernel/proc.h`).

## Parte 4: consulta y pruebas

**Makefile**: se agregan `$U/_pstate` y `$U/_suspendtest` a `UPROGS` para que se compilen y se copien a `fs.img`; sin esto no existen como comandos en el shell.

**Instrucciones**:

```
make clean && make qemu
$ pstate 1 2
$ suspendtest
```

**Salida esperada**:

```
PID     ESTADO
1       sleeping
2       sleeping

OK  : 1. suspender proceso en ejecucion
OK  : 2. reanudar proceso suspendido
OK  : 3. suspender dos veces da error
OK  : 4. no se puede suspender init
OK  : 5. pid inexistente da error
OK  : 6. reanudar proceso no suspendido da error
OK  : 7. matar proceso suspendido
OK  : 8. suspension pendiente de proceso dormido

8 OK, 0 FAIL
```

## Relación con Linux

Equivale a `kill -STOP <pid>` / `kill -CONT <pid>` (señales `SIGSTOP` y `SIGCONT`). En Linux el proceso detenido aparece con estado `T` en `ps`, como aquí aparece `suspended` en `pstate`.
