(define map
  (lambda (f L)
    (if (null? L)
        L
        (cons (f (car L))
              (map f (cdr L))))))
