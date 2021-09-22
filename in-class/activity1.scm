(define nth
  (lambda (n   list)
    (cond ((null? list) '())
          ((equal? n 1) (car list))
          (else (nth (- n 1) (cdr list))))))

(define remove-first
  (lambda (element list)
    (cond ((null? list) list)
          ((equal? (car list) element) (cdr list))
          (else (cons (car list) (remove-first element (cdr list)))))))

(define remove-all
  (lambda (element list)
    (cond ((null? list) list)
          ((equal? (car list) element) (remove-all element (cdr list)))
          (else (cons (car list) (remove-all element (cdr list)))))))

(remove-all '4 '(1 2 3 4 5 4 4 6))
