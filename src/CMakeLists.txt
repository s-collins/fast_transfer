add_library(fast_transfer
    fast_transfer/ft_impl/buffer.c
    fast_transfer/ft_impl/crc.c
    fast_transfer/ft_impl/ft.c
    fast_transfer/ft_impl/parser.c
)
target_include_directories(fast_transfer
    PUBLIC
        ./fast_transfer
        ./fast_transfer/ft_impl
)
