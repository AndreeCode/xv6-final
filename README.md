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
$ spin
```

Retornan `0` si la operación es correcta, o un código negativo `SUSP_*` (ver `kernel/proc.h`).

## Cómo probar

**Makefile**: `_suspend`, `_resume`, `_pstate`, `_suspendtest` y `_spin` están en `UPROGS` para que se compilen y se copien a `fs.img`.

### 1. Compilar y ejecutar

```
make clean
make qemu
```

`make clean` borra lo compilado antes; `make qemu` compila todo y arranca xv6 en QEMU.

### 2. Pruebas automáticas

```
$ suspendtest
```

Ejecuta 8 pruebas que suspenden, reanudan y matan procesos, y revisan su estado con `pstate`. Salida esperada:

```
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

### 3. Demo manual

| Comando | Qué hace |
|---|---|
| `spin &` | Arranca un proceso que imprime un contador cada 2 s |
| `pstate` | Muestra los procesos y su estado (spin suele ser el PID 4) |
| `suspend 4` | Suspende spin: deja de imprimir |
| `pstate 4` | Muestra `4  suspended` |
| `resume 4` | Reanuda spin: sigue contando donde quedó |
| `kill 4` | Termina spin |

### 4. Casos de error

| Comando | Resultado | Motivo |
|---|---|---|
| `suspend 1` | código `-2` | No se puede suspender `init` |
| `suspend 999` | código `-1` | El PID no existe |
| `resume 2` | código `-6` | El proceso no está suspendido |
| `suspend abc` | `PID invalido` | El argumento no es un número |

### 5. Salir

`Ctrl+a` y luego `x`.

## Relación con Linux

Equivale a `kill -STOP <pid>` / `kill -CONT <pid>` (señales `SIGSTOP` y `SIGCONT`). En Linux el proceso detenido aparece con estado `T` en `ps`, como aquí aparece `suspended` en `pstate`.
