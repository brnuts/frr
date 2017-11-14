/*
 * kernel routing table update prototype.
 * Copyright (C) 1998 Kunihiro Ishiguro
 *
 * This file is part of GNU Zebra.
 *
 * GNU Zebra is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2, or (at your option) any
 * later version.
 *
 * GNU Zebra is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; see the file COPYING; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
 */

#ifndef _ZEBRA_RT_H
#define _ZEBRA_RT_H

#include "prefix.h"
#include "if.h"
#include "vlan.h"
#include "vxlan.h"
#include "zebra/rib.h"
#include "zebra/zebra_ns.h"
#include "zebra/zebra_mpls.h"

/*
 * Philosophy Note:
 *
 * Flags being SET/UNSET do not belong in the South Bound
 * Interface.  This Setting belongs at the calling level
 * because we can and will have multiple different interfaces
 * and we will have potentially multiple different
 * modules/filters to call.  As such Setting/Unsetting
 * success failure should be handled by the caller.
 */


enum southbound_results {
	SOUTHBOUND_INSTALL_SUCCESS,
	SOUTHBOUND_INSTALL_FAILURE,
	SOUTHBOUND_DELETE_SUCCESS,
	SOUTHBOUND_DELETE_FAILURE,
};

/*
 * Install/delete the specified prefix p from the kernel
 *
 * old = NULL, new = pointer - Install new
 * old = pointer, new = pointer - Route replace Old w/ New
 * old = pointer, new = NULL, - Route Delete
 *
 * Please note not all kernels support route replace
 * semantics so we will end up with a delete than
 * a re-add.
 */
extern void kernel_route_rib(struct prefix *p, struct prefix *src_p,
			     struct route_entry *old, struct route_entry *new);

/*
 * So route install/failure may not be immediately known
 * so let's separate it out and allow the result to
 * be passed back up.
 */
extern void kernel_route_rib_pass_fail(struct prefix *p,
				       struct route_entry *re,
				       enum southbound_results res);

extern int kernel_address_add_ipv4(struct interface *, struct connected *);
extern int kernel_address_delete_ipv4(struct interface *, struct connected *);
extern int kernel_neigh_update(int, int, uint32_t, char *, int);
extern int kernel_interface_set_master(struct interface *master,
				       struct interface *slave);
extern int kernel_add_lsp(zebra_lsp_t *);
extern int kernel_upd_lsp(zebra_lsp_t *);
extern int kernel_del_lsp(zebra_lsp_t *);
extern int mpls_kernel_init(void);

extern int kernel_get_ipmr_sg_stats(struct zebra_vrf *zvrf, void *mroute);
extern int kernel_add_vtep(vni_t vni, struct interface *ifp,
			   struct in_addr *vtep_ip);
extern int kernel_del_vtep(vni_t vni, struct interface *ifp,
			   struct in_addr *vtep_ip);
extern int kernel_add_mac(struct interface *ifp, vlanid_t vid,
			  struct ethaddr *mac, struct in_addr vtep_ip,
			  u_char sticky);
extern int kernel_del_mac(struct interface *ifp, vlanid_t vid,
			  struct ethaddr *mac, struct in_addr vtep_ip,
			  int local);

extern int kernel_add_neigh(struct interface *ifp, struct ipaddr *ip,
			    struct ethaddr *mac);
extern int kernel_del_neigh(struct interface *ifp, struct ipaddr *ip);

#endif /* _ZEBRA_RT_H */
