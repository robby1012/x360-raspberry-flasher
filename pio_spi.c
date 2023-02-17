/*
 * Copyright (c) 2022 Robby Sitanala <robby1412@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "pio_spi.h"
#include "hardware/clocks.h"

// Just 8 bit functions provided here. The PIO program supports any frame size
// 1...32, but the software to do the necessary FIFO shuffling is left as an
// exercise for the reader :)
//
// Likewise we only provide MSB-first here. To do LSB-first, you need to
// - Do shifts when reading from the FIFO, for general case n != 8, 16, 32
// - Do a narrow read at a one halfword or 3 byte offset for n == 16, 8
// in order to get the read data correctly justified.

void pio_spi_init(pio_spi_inst_t *spi, PIO pio, uint sm, float freq, uint n_bits, pio_spi_order_t order, bool cpha, bool cpol, uint pin_ss, uint pin_mosi, uint pin_miso)
{
	spi->pio = pio;
	spi->sm = sm;
	spi->order = order;

	if (!cpha)
		spi->prog = pio_add_program(spi->pio, &spi_cpha0_cs_program);
	else
		spi->prog = pio_add_program(spi->pio, &spi_cpha1_cs_program);

	float clockdiv = clock_get_hz(clk_sys);
	clockdiv /= 4000000.f;

	clockdiv /= freq;

	pio_spi_cs_init(pio, sm, spi->prog, n_bits, clockdiv, cpha, cpol, pin_ss, pin_mosi, pin_miso, order);
}

void __time_critical_func(pio_spi_write8_blocking)(const pio_spi_inst_t *spi, const uint8_t *src, size_t len)
{
	size_t tx_remain = len, rx_remain = len;
	// Do 8 bit accesses on FIFO, so that write data is byte-replicated. This
	// gets us the left-justification for free (for MSB-first shift-out)
	io_rw_8 *txfifo = (io_rw_8 *)&spi->pio->txf[spi->sm];
	io_rw_8 *rxfifo = (io_rw_8 *)&spi->pio->rxf[spi->sm];
	while (tx_remain || rx_remain)
	{
		if (tx_remain && !pio_sm_is_tx_fifo_full(spi->pio, spi->sm))
		{
			*txfifo = *src++;
			--tx_remain;
		}
		if (rx_remain && !pio_sm_is_rx_fifo_empty(spi->pio, spi->sm))
		{
			(void)*rxfifo;
			--rx_remain;
		}
	}
}

void __time_critical_func(pio_spi_read8_blocking)(const pio_spi_inst_t *spi, uint8_t *dst, size_t len)
{
	size_t tx_remain = len, rx_remain = len;
	io_rw_8 *txfifo = (io_rw_8 *)&spi->pio->txf[spi->sm];
	io_rw_8 *rxfifo = (io_rw_8 *)&spi->pio->rxf[spi->sm];
	if (spi->order == SPI_LSB_FIRST)
		rxfifo += 3;
	while (tx_remain || rx_remain)
	{
		if (tx_remain && !pio_sm_is_tx_fifo_full(spi->pio, spi->sm))
		{
			*txfifo = 0;
			--tx_remain;
		}
		if (rx_remain && !pio_sm_is_rx_fifo_empty(spi->pio, spi->sm))
		{
			*dst++ = *rxfifo;
			-- rx_remain;
		}
	}
}

void __time_critical_func(pio_spi_write8_read8_blocking)(const pio_spi_inst_t *spi, uint8_t *src, uint8_t *dst,
														 size_t len)
{
	size_t tx_remain = len, rx_remain = len;
	io_rw_8 *txfifo = (io_rw_8 *)&spi->pio->txf[spi->sm];
	io_rw_8 *rxfifo = (io_rw_8 *)&spi->pio->rxf[spi->sm];
	if (spi->order == SPI_LSB_FIRST)
		rxfifo += 3;
	while (tx_remain || rx_remain)
	{
		if (tx_remain && !pio_sm_is_tx_fifo_full(spi->pio, spi->sm))
		{
			*txfifo = *src++;
			--tx_remain;
		}
		if (rx_remain && !pio_sm_is_rx_fifo_empty(spi->pio, spi->sm))
		{
			*dst++ = *rxfifo;
			--rx_remain;
		}
	}
}
