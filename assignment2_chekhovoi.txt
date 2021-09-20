(define sum
  (lambda (n)
    (if (null? n )
      0
      (+ (car n) (sum (cdr n)))
    )
  )
)

(sum '())
