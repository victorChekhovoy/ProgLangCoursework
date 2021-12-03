(define reverse-custom
  (lambda (lst)
    (if (null? lst)
      	'()
	(append (reverse-custom (cdr lst)) (list (car lst))))))

(reverse-custom '(1 2 3 4 5))

