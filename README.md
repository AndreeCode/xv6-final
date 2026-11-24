# xv6riscv-process-suspend-resume

Suspensión y reanudación de procesos por PID en xv6-riscv.

## Avance

1. **Estado `SUSPENDED`**: nuevo estado en `enum procstate`; el planificador no ejecuta procesos suspendidos.
2. **Suspensión (`ksuspend`)**: marca el proceso (`suspend_req`) y este pasa a `SUSPENDED` en un punto seguro (`suspend_check()`, al volver a modo usuario, sin locks del kernel). No se puede suspender a `init`.
3. **Reanudación (`kresume`)**: el proceso vuelve a `RUNNABLE`, o se cancela la suspensión pendiente. Llamadas al sistema `suspend(pid)` y `resume(pid)`, y los comandos del shell `suspend` y `resume`.

## Uso

```
make qemu
$ suspend <pid>
$ resume <pid>
```

Retornan `0` si la operación es correcta, o un código negativo `SUSP_*` (ver `kernel/proc.h`).
