(define keep-first-n (
  lambda (n m) (
    (let output)
    (if (<= n 0)
      ()
      (
        (const (car m) output)
        (keep-first-n (- n 1) (cdr m))
      )

    )
    )
  )
)

(keep-first-n 3 '(a b c d e f g))
