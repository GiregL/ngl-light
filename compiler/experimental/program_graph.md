# Program Graph

## Constitution

Example code
```ngl
ngl::function add
{
    ngl::int <a>
    ngl::int <b>
    
    .result (a + b)
}

ngl::shape addition
{
    ngl::sequence<ngl::identifier, ngl::plus, ngl::identifier>
//                      a               +           b
}

ngl:edge<ngl, addition, add>
```

Graph:
```
[ngl::function:add] ------------------ [ngl::shape:addition]
 [ngl::int "a"   ]                      [                 ]
 [ngl::int "b"   ]                      [                 ]
 [.result ???    ]                      [                 ]
```

## Algorithms