/* this file is generated - do not edit */

#include <cstdint>

static const uint8_t crc8table[256] = {
	0x00, 0x5e, 0xbc, 0xe2, 0x61, 0x3f, 0xdd, 0x83, 0xc2, 0x9c, 0x7e, 0x20, 0xa3, 0xfd, 0x1f,
	0x41, 0x9d, 0xc3, 0x21, 0x7f, 0xfc, 0xa2, 0x40, 0x1e, 0x5f, 0x01, 0xe3, 0xbd, 0x3e, 0x60,
	0x82, 0xdc, 0x23, 0x7d, 0x9f, 0xc1, 0x42, 0x1c, 0xfe, 0xa0, 0xe1, 0xbf, 0x5d, 0x03, 0x80,
	0xde, 0x3c, 0x62, 0xbe, 0xe0, 0x02, 0x5c, 0xdf, 0x81, 0x63, 0x3d, 0x7c, 0x22, 0xc0, 0x9e,
	0x1d, 0x43, 0xa1, 0xff, 0x46, 0x18, 0xfa, 0xa4, 0x27, 0x79, 0x9b, 0xc5, 0x84, 0xda, 0x38,
	0x66, 0xe5, 0xbb, 0x59, 0x07, 0xdb, 0x85, 0x67, 0x39, 0xba, 0xe4, 0x06, 0x58, 0x19, 0x47,
	0xa5, 0xfb, 0x78, 0x26, 0xc4, 0x9a, 0x65, 0x3b, 0xd9, 0x87, 0x04, 0x5a, 0xb8, 0xe6, 0xa7,
	0xf9, 0x1b, 0x45, 0xc6, 0x98, 0x7a, 0x24, 0xf8, 0xa6, 0x44, 0x1a, 0x99, 0xc7, 0x25, 0x7b,
	0x3a, 0x64, 0x86, 0xd8, 0x5b, 0x05, 0xe7, 0xb9, 0x8c, 0xd2, 0x30, 0x6e, 0xed, 0xb3, 0x51,
	0x0f, 0x4e, 0x10, 0xf2, 0xac, 0x2f, 0x71, 0x93, 0xcd, 0x11, 0x4f, 0xad, 0xf3, 0x70, 0x2e,
	0xcc, 0x92, 0xd3, 0x8d, 0x6f, 0x31, 0xb2, 0xec, 0x0e, 0x50, 0xaf, 0xf1, 0x13, 0x4d, 0xce,
	0x90, 0x72, 0x2c, 0x6d, 0x33, 0xd1, 0x8f, 0x0c, 0x52, 0xb0, 0xee, 0x32, 0x6c, 0x8e, 0xd0,
	0x53, 0x0d, 0xef, 0xb1, 0xf0, 0xae, 0x4c, 0x12, 0x91, 0xcf, 0x2d, 0x73, 0xca, 0x94, 0x76,
	0x28, 0xab, 0xf5, 0x17, 0x49, 0x08, 0x56, 0xb4, 0xea, 0x69, 0x37, 0xd5, 0x8b, 0x57, 0x09,
	0xeb, 0xb5, 0x36, 0x68, 0x8a, 0xd4, 0x95, 0xcb, 0x29, 0x77, 0xf4, 0xaa, 0x48, 0x16, 0xe9,
	0xb7, 0x55, 0x0b, 0x88, 0xd6, 0x34, 0x6a, 0x2b, 0x75, 0x97, 0xc9, 0x4a, 0x14, 0xf6, 0xa8,
	0x74, 0x2a, 0xc8, 0x96, 0x15, 0x4b, 0xa9, 0xf7, 0xb6, 0xe8, 0x0a, 0x54, 0xd7, 0x89, 0x6b, 0x35
};

static const uint64_t crc64table[256] = {
	0x0000000000000000ULL, 	0x42f0e1eba9ea3693ULL,
	0x85e1c3d753d46d26ULL, 	0xc711223cfa3e5bb5ULL,
	0x493366450e42ecdfULL, 	0x0bc387aea7a8da4cULL,
	0xccd2a5925d9681f9ULL, 	0x8e224479f47cb76aULL,
	0x9266cc8a1c85d9beULL, 	0xd0962d61b56fef2dULL,
	0x17870f5d4f51b498ULL, 	0x5577eeb6e6bb820bULL,
	0xdb55aacf12c73561ULL, 	0x99a54b24bb2d03f2ULL,
	0x5eb4691841135847ULL, 	0x1c4488f3e8f96ed4ULL,
	0x663d78ff90e185efULL, 	0x24cd9914390bb37cULL,
	0xe3dcbb28c335e8c9ULL, 	0xa12c5ac36adfde5aULL,
	0x2f0e1eba9ea36930ULL, 	0x6dfeff5137495fa3ULL,
	0xaaefdd6dcd770416ULL, 	0xe81f3c86649d3285ULL,
	0xf45bb4758c645c51ULL, 	0xb6ab559e258e6ac2ULL,
	0x71ba77a2dfb03177ULL, 	0x334a9649765a07e4ULL,
	0xbd68d2308226b08eULL, 	0xff9833db2bcc861dULL,
	0x388911e7d1f2dda8ULL, 	0x7a79f00c7818eb3bULL,
	0xcc7af1ff21c30bdeULL, 	0x8e8a101488293d4dULL,
	0x499b3228721766f8ULL, 	0x0b6bd3c3dbfd506bULL,
	0x854997ba2f81e701ULL, 	0xc7b97651866bd192ULL,
	0x00a8546d7c558a27ULL, 	0x4258b586d5bfbcb4ULL,
	0x5e1c3d753d46d260ULL, 	0x1cecdc9e94ace4f3ULL,
	0xdbfdfea26e92bf46ULL, 	0x990d1f49c77889d5ULL,
	0x172f5b3033043ebfULL, 	0x55dfbadb9aee082cULL,
	0x92ce98e760d05399ULL, 	0xd03e790cc93a650aULL,
	0xaa478900b1228e31ULL, 	0xe8b768eb18c8b8a2ULL,
	0x2fa64ad7e2f6e317ULL, 	0x6d56ab3c4b1cd584ULL,
	0xe374ef45bf6062eeULL, 	0xa1840eae168a547dULL,
	0x66952c92ecb40fc8ULL, 	0x2465cd79455e395bULL,
	0x3821458aada7578fULL, 	0x7ad1a461044d611cULL,
	0xbdc0865dfe733aa9ULL, 	0xff3067b657990c3aULL,
	0x711223cfa3e5bb50ULL, 	0x33e2c2240a0f8dc3ULL,
	0xf4f3e018f031d676ULL, 	0xb60301f359dbe0e5ULL,
	0xda050215ea6c212fULL, 	0x98f5e3fe438617bcULL,
	0x5fe4c1c2b9b84c09ULL, 	0x1d14202910527a9aULL,
	0x93366450e42ecdf0ULL, 	0xd1c685bb4dc4fb63ULL,
	0x16d7a787b7faa0d6ULL, 	0x5427466c1e109645ULL,
	0x4863ce9ff6e9f891ULL, 	0x0a932f745f03ce02ULL,
	0xcd820d48a53d95b7ULL, 	0x8f72eca30cd7a324ULL,
	0x0150a8daf8ab144eULL, 	0x43a04931514122ddULL,
	0x84b16b0dab7f7968ULL, 	0xc6418ae602954ffbULL,
	0xbc387aea7a8da4c0ULL, 	0xfec89b01d3679253ULL,
	0x39d9b93d2959c9e6ULL, 	0x7b2958d680b3ff75ULL,
	0xf50b1caf74cf481fULL, 	0xb7fbfd44dd257e8cULL,
	0x70eadf78271b2539ULL, 	0x321a3e938ef113aaULL,
	0x2e5eb66066087d7eULL, 	0x6cae578bcfe24bedULL,
	0xabbf75b735dc1058ULL, 	0xe94f945c9c3626cbULL,
	0x676dd025684a91a1ULL, 	0x259d31cec1a0a732ULL,
	0xe28c13f23b9efc87ULL, 	0xa07cf2199274ca14ULL,
	0x167ff3eacbaf2af1ULL, 	0x548f120162451c62ULL,
	0x939e303d987b47d7ULL, 	0xd16ed1d631917144ULL,
	0x5f4c95afc5edc62eULL, 	0x1dbc74446c07f0bdULL,
	0xdaad56789639ab08ULL, 	0x985db7933fd39d9bULL,
	0x84193f60d72af34fULL, 	0xc6e9de8b7ec0c5dcULL,
	0x01f8fcb784fe9e69ULL, 	0x43081d5c2d14a8faULL,
	0xcd2a5925d9681f90ULL, 	0x8fdab8ce70822903ULL,
	0x48cb9af28abc72b6ULL, 	0x0a3b7b1923564425ULL,
	0x70428b155b4eaf1eULL, 	0x32b26afef2a4998dULL,
	0xf5a348c2089ac238ULL, 	0xb753a929a170f4abULL,
	0x3971ed50550c43c1ULL, 	0x7b810cbbfce67552ULL,
	0xbc902e8706d82ee7ULL, 	0xfe60cf6caf321874ULL,
	0xe224479f47cb76a0ULL, 	0xa0d4a674ee214033ULL,
	0x67c58448141f1b86ULL, 	0x253565a3bdf52d15ULL,
	0xab1721da49899a7fULL, 	0xe9e7c031e063acecULL,
	0x2ef6e20d1a5df759ULL, 	0x6c0603e6b3b7c1caULL,
	0xf6fae5c07d3274cdULL, 	0xb40a042bd4d8425eULL,
	0x731b26172ee619ebULL, 	0x31ebc7fc870c2f78ULL,
	0xbfc9838573709812ULL, 	0xfd39626eda9aae81ULL,
	0x3a28405220a4f534ULL, 	0x78d8a1b9894ec3a7ULL,
	0x649c294a61b7ad73ULL, 	0x266cc8a1c85d9be0ULL,
	0xe17dea9d3263c055ULL, 	0xa38d0b769b89f6c6ULL,
	0x2daf4f0f6ff541acULL, 	0x6f5faee4c61f773fULL,
	0xa84e8cd83c212c8aULL, 	0xeabe6d3395cb1a19ULL,
	0x90c79d3fedd3f122ULL, 	0xd2377cd44439c7b1ULL,
	0x15265ee8be079c04ULL, 	0x57d6bf0317edaa97ULL,
	0xd9f4fb7ae3911dfdULL, 	0x9b041a914a7b2b6eULL,
	0x5c1538adb04570dbULL, 	0x1ee5d94619af4648ULL,
	0x02a151b5f156289cULL, 	0x4051b05e58bc1e0fULL,
	0x87409262a28245baULL, 	0xc5b073890b687329ULL,
	0x4b9237f0ff14c443ULL, 	0x0962d61b56fef2d0ULL,
	0xce73f427acc0a965ULL, 	0x8c8315cc052a9ff6ULL,
	0x3a80143f5cf17f13ULL, 	0x7870f5d4f51b4980ULL,
	0xbf61d7e80f251235ULL, 	0xfd913603a6cf24a6ULL,
	0x73b3727a52b393ccULL, 	0x31439391fb59a55fULL,
	0xf652b1ad0167feeaULL, 	0xb4a25046a88dc879ULL,
	0xa8e6d8b54074a6adULL, 	0xea16395ee99e903eULL,
	0x2d071b6213a0cb8bULL, 	0x6ff7fa89ba4afd18ULL,
	0xe1d5bef04e364a72ULL, 	0xa3255f1be7dc7ce1ULL,
	0x64347d271de22754ULL, 	0x26c49cccb40811c7ULL,
	0x5cbd6cc0cc10fafcULL, 	0x1e4d8d2b65facc6fULL,
	0xd95caf179fc497daULL, 	0x9bac4efc362ea149ULL,
	0x158e0a85c2521623ULL, 	0x577eeb6e6bb820b0ULL,
	0x906fc95291867b05ULL, 	0xd29f28b9386c4d96ULL,
	0xcedba04ad0952342ULL, 	0x8c2b41a1797f15d1ULL,
	0x4b3a639d83414e64ULL, 	0x09ca82762aab78f7ULL,
	0x87e8c60fded7cf9dULL, 	0xc51827e4773df90eULL,
	0x020905d88d03a2bbULL, 	0x40f9e43324e99428ULL,
	0x2cffe7d5975e55e2ULL, 	0x6e0f063e3eb46371ULL,
	0xa91e2402c48a38c4ULL, 	0xebeec5e96d600e57ULL,
	0x65cc8190991cb93dULL, 	0x273c607b30f68faeULL,
	0xe02d4247cac8d41bULL, 	0xa2dda3ac6322e288ULL,
	0xbe992b5f8bdb8c5cULL, 	0xfc69cab42231bacfULL,
	0x3b78e888d80fe17aULL, 	0x7988096371e5d7e9ULL,
	0xf7aa4d1a85996083ULL, 	0xb55aacf12c735610ULL,
	0x724b8ecdd64d0da5ULL, 	0x30bb6f267fa73b36ULL,
	0x4ac29f2a07bfd00dULL, 	0x08327ec1ae55e69eULL,
	0xcf235cfd546bbd2bULL, 	0x8dd3bd16fd818bb8ULL,
	0x03f1f96f09fd3cd2ULL, 	0x41011884a0170a41ULL,
	0x86103ab85a2951f4ULL, 	0xc4e0db53f3c36767ULL,
	0xd8a453a01b3a09b3ULL, 	0x9a54b24bb2d03f20ULL,
	0x5d45907748ee6495ULL, 	0x1fb5719ce1045206ULL,
	0x919735e51578e56cULL, 	0xd367d40ebc92d3ffULL,
	0x1476f63246ac884aULL, 	0x568617d9ef46bed9ULL,
	0xe085162ab69d5e3cULL, 	0xa275f7c11f7768afULL,
	0x6564d5fde549331aULL, 	0x279434164ca30589ULL,
	0xa9b6706fb8dfb2e3ULL, 	0xeb46918411358470ULL,
	0x2c57b3b8eb0bdfc5ULL, 	0x6ea7525342e1e956ULL,
	0x72e3daa0aa188782ULL, 	0x30133b4b03f2b111ULL,
	0xf7021977f9cceaa4ULL, 	0xb5f2f89c5026dc37ULL,
	0x3bd0bce5a45a6b5dULL, 	0x79205d0e0db05dceULL,
	0xbe317f32f78e067bULL, 	0xfcc19ed95e6430e8ULL,
	0x86b86ed5267cdbd3ULL, 	0xc4488f3e8f96ed40ULL,
	0x0359ad0275a8b6f5ULL, 	0x41a94ce9dc428066ULL,
	0xcf8b0890283e370cULL, 	0x8d7be97b81d4019fULL,
	0x4a6acb477bea5a2aULL, 	0x089a2aacd2006cb9ULL,
	0x14dea25f3af9026dULL, 	0x562e43b4931334feULL,
	0x913f6188692d6f4bULL, 	0xd3cf8063c0c759d8ULL,
	0x5dedc41a34bbeeb2ULL, 	0x1f1d25f19d51d821ULL,
	0xd80c07cd676f8394ULL, 	0x9afce626ce85b507ULL,
};

static const uint64_t crc64rocksofttable[256] = {
	0x0000000000000000ULL, 	0x7f6ef0c830358979ULL,
	0xfedde190606b12f2ULL, 	0x81b31158505e9b8bULL,
	0xc962e5739841b68fULL, 	0xb60c15bba8743ff6ULL,
	0x37bf04e3f82aa47dULL, 	0x48d1f42bc81f2d04ULL,
	0xa61cecb46814fe75ULL, 	0xd9721c7c5821770cULL,
	0x58c10d24087fec87ULL, 	0x27affdec384a65feULL,
	0x6f7e09c7f05548faULL, 	0x1010f90fc060c183ULL,
	0x91a3e857903e5a08ULL, 	0xeecd189fa00bd371ULL,
	0x78e0ff3b88be6f81ULL, 	0x078e0ff3b88be6f8ULL,
	0x863d1eabe8d57d73ULL, 	0xf953ee63d8e0f40aULL,
	0xb1821a4810ffd90eULL, 	0xceecea8020ca5077ULL,
	0x4f5ffbd87094cbfcULL, 	0x30310b1040a14285ULL,
	0xdefc138fe0aa91f4ULL, 	0xa192e347d09f188dULL,
	0x2021f21f80c18306ULL, 	0x5f4f02d7b0f40a7fULL,
	0x179ef6fc78eb277bULL, 	0x68f0063448deae02ULL,
	0xe943176c18803589ULL, 	0x962de7a428b5bcf0ULL,
	0xf1c1fe77117cdf02ULL, 	0x8eaf0ebf2149567bULL,
	0x0f1c1fe77117cdf0ULL, 	0x7072ef2f41224489ULL,
	0x38a31b04893d698dULL, 	0x47cdebccb908e0f4ULL,
	0xc67efa94e9567b7fULL, 	0xb9100a5cd963f206ULL,
	0x57dd12c379682177ULL, 	0x28b3e20b495da80eULL,
	0xa900f35319033385ULL, 	0xd66e039b2936bafcULL,
	0x9ebff7b0e12997f8ULL, 	0xe1d10778d11c1e81ULL,
	0x606216208142850aULL, 	0x1f0ce6e8b1770c73ULL,
	0x8921014c99c2b083ULL, 	0xf64ff184a9f739faULL,
	0x77fce0dcf9a9a271ULL, 	0x08921014c99c2b08ULL,
	0x4043e43f0183060cULL, 	0x3f2d14f731b68f75ULL,
	0xbe9e05af61e814feULL, 	0xc1f0f56751dd9d87ULL,
	0x2f3dedf8f1d64ef6ULL, 	0x50531d30c1e3c78fULL,
	0xd1e00c6891bd5c04ULL, 	0xae8efca0a188d57dULL,
	0xe65f088b6997f879ULL, 	0x9931f84359a27100ULL,
	0x1882e91b09fcea8bULL, 	0x67ec19d339c963f2ULL,
	0xd75adabd7a6e2d6fULL, 	0xa8342a754a5ba416ULL,
	0x29873b2d1a053f9dULL, 	0x56e9cbe52a30b6e4ULL,
	0x1e383fcee22f9be0ULL, 	0x6156cf06d21a1299ULL,
	0xe0e5de5e82448912ULL, 	0x9f8b2e96b271006bULL,
	0x71463609127ad31aULL, 	0x0e28c6c1224f5a63ULL,
	0x8f9bd7997211c1e8ULL, 	0xf0f5275142244891ULL,
	0xb824d37a8a3b6595ULL, 	0xc74a23b2ba0eececULL,
	0x46f932eaea507767ULL, 	0x3997c222da65fe1eULL,
	0xafba2586f2d042eeULL, 	0xd0d4d54ec2e5cb97ULL,
	0x5167c41692bb501cULL, 	0x2e0934dea28ed965ULL,
	0x66d8c0f56a91f461ULL, 	0x19b6303d5aa47d18ULL,
	0x980521650afae693ULL, 	0xe76bd1ad3acf6feaULL,
	0x09a6c9329ac4bc9bULL, 	0x76c839faaaf135e2ULL,
	0xf77b28a2faafae69ULL, 	0x8815d86aca9a2710ULL,
	0xc0c42c4102850a14ULL, 	0xbfaadc8932b0836dULL,
	0x3e19cdd162ee18e6ULL, 	0x41773d1952db919fULL,
	0x269b24ca6b12f26dULL, 	0x59f5d4025b277b14ULL,
	0xd846c55a0b79e09fULL, 	0xa72835923b4c69e6ULL,
	0xeff9c1b9f35344e2ULL, 	0x90973171c366cd9bULL,
	0x1124202993385610ULL, 	0x6e4ad0e1a30ddf69ULL,
	0x8087c87e03060c18ULL, 	0xffe938b633338561ULL,
	0x7e5a29ee636d1eeaULL, 	0x0134d92653589793ULL,
	0x49e52d0d9b47ba97ULL, 	0x368bddc5ab7233eeULL,
	0xb738cc9dfb2ca865ULL, 	0xc8563c55cb19211cULL,
	0x5e7bdbf1e3ac9decULL, 	0x21152b39d3991495ULL,
	0xa0a63a6183c78f1eULL, 	0xdfc8caa9b3f20667ULL,
	0x97193e827bed2b63ULL, 	0xe877ce4a4bd8a21aULL,
	0x69c4df121b863991ULL, 	0x16aa2fda2bb3b0e8ULL,
	0xf86737458bb86399ULL, 	0x8709c78dbb8deae0ULL,
	0x06bad6d5ebd3716bULL, 	0x79d4261ddbe6f812ULL,
	0x3105d23613f9d516ULL, 	0x4e6b22fe23cc5c6fULL,
	0xcfd833a67392c7e4ULL, 	0xb0b6c36e43a74e9dULL,
	0x9a6c9329ac4bc9b5ULL, 	0xe50263e19c7e40ccULL,
	0x64b172b9cc20db47ULL, 	0x1bdf8271fc15523eULL,
	0x530e765a340a7f3aULL, 	0x2c608692043ff643ULL,
	0xadd397ca54616dc8ULL, 	0xd2bd67026454e4b1ULL,
	0x3c707f9dc45f37c0ULL, 	0x431e8f55f46abeb9ULL,
	0xc2ad9e0da4342532ULL, 	0xbdc36ec59401ac4bULL,
	0xf5129aee5c1e814fULL, 	0x8a7c6a266c2b0836ULL,
	0x0bcf7b7e3c7593bdULL, 	0x74a18bb60c401ac4ULL,
	0xe28c6c1224f5a634ULL, 	0x9de29cda14c02f4dULL,
	0x1c518d82449eb4c6ULL, 	0x633f7d4a74ab3dbfULL,
	0x2bee8961bcb410bbULL, 	0x548079a98c8199c2ULL,
	0xd53368f1dcdf0249ULL, 	0xaa5d9839ecea8b30ULL,
	0x449080a64ce15841ULL, 	0x3bfe706e7cd4d138ULL,
	0xba4d61362c8a4ab3ULL, 	0xc52391fe1cbfc3caULL,
	0x8df265d5d4a0eeceULL, 	0xf29c951de49567b7ULL,
	0x732f8445b4cbfc3cULL, 	0x0c41748d84fe7545ULL,
	0x6bad6d5ebd3716b7ULL, 	0x14c39d968d029fceULL,
	0x95708ccedd5c0445ULL, 	0xea1e7c06ed698d3cULL,
	0xa2cf882d2576a038ULL, 	0xdda178e515432941ULL,
	0x5c1269bd451db2caULL, 	0x237c997575283bb3ULL,
	0xcdb181ead523e8c2ULL, 	0xb2df7122e51661bbULL,
	0x336c607ab548fa30ULL, 	0x4c0290b2857d7349ULL,
	0x04d364994d625e4dULL, 	0x7bbd94517d57d734ULL,
	0xfa0e85092d094cbfULL, 	0x856075c11d3cc5c6ULL,
	0x134d926535897936ULL, 	0x6c2362ad05bcf04fULL,
	0xed9073f555e26bc4ULL, 	0x92fe833d65d7e2bdULL,
	0xda2f7716adc8cfb9ULL, 	0xa54187de9dfd46c0ULL,
	0x24f29686cda3dd4bULL, 	0x5b9c664efd965432ULL,
	0xb5517ed15d9d8743ULL, 	0xca3f8e196da80e3aULL,
	0x4b8c9f413df695b1ULL, 	0x34e26f890dc31cc8ULL,
	0x7c339ba2c5dc31ccULL, 	0x035d6b6af5e9b8b5ULL,
	0x82ee7a32a5b7233eULL, 	0xfd808afa9582aa47ULL,
	0x4d364994d625e4daULL, 	0x3258b95ce6106da3ULL,
	0xb3eba804b64ef628ULL, 	0xcc8558cc867b7f51ULL,
	0x8454ace74e645255ULL, 	0xfb3a5c2f7e51db2cULL,
	0x7a894d772e0f40a7ULL, 	0x05e7bdbf1e3ac9deULL,
	0xeb2aa520be311aafULL, 	0x944455e88e0493d6ULL,
	0x15f744b0de5a085dULL, 	0x6a99b478ee6f8124ULL,
	0x224840532670ac20ULL, 	0x5d26b09b16452559ULL,
	0xdc95a1c3461bbed2ULL, 	0xa3fb510b762e37abULL,
	0x35d6b6af5e9b8b5bULL, 	0x4ab846676eae0222ULL,
	0xcb0b573f3ef099a9ULL, 	0xb465a7f70ec510d0ULL,
	0xfcb453dcc6da3dd4ULL, 	0x83daa314f6efb4adULL,
	0x0269b24ca6b12f26ULL, 	0x7d0742849684a65fULL,
	0x93ca5a1b368f752eULL, 	0xeca4aad306bafc57ULL,
	0x6d17bb8b56e467dcULL, 	0x12794b4366d1eea5ULL,
	0x5aa8bf68aecec3a1ULL, 	0x25c64fa09efb4ad8ULL,
	0xa4755ef8cea5d153ULL, 	0xdb1bae30fe90582aULL,
	0xbcf7b7e3c7593bd8ULL, 	0xc399472bf76cb2a1ULL,
	0x422a5673a732292aULL, 	0x3d44a6bb9707a053ULL,
	0x759552905f188d57ULL, 	0x0afba2586f2d042eULL,
	0x8b48b3003f739fa5ULL, 	0xf42643c80f4616dcULL,
	0x1aeb5b57af4dc5adULL, 	0x6585ab9f9f784cd4ULL,
	0xe436bac7cf26d75fULL, 	0x9b584a0fff135e26ULL,
	0xd389be24370c7322ULL, 	0xace74eec0739fa5bULL,
	0x2d545fb4576761d0ULL, 	0x523aaf7c6752e8a9ULL,
	0xc41748d84fe75459ULL, 	0xbb79b8107fd2dd20ULL,
	0x3acaa9482f8c46abULL, 	0x45a459801fb9cfd2ULL,
	0x0d75adabd7a6e2d6ULL, 	0x721b5d63e7936bafULL,
	0xf3a84c3bb7cdf024ULL, 	0x8cc6bcf387f8795dULL,
	0x620ba46c27f3aa2cULL, 	0x1d6554a417c62355ULL,
	0x9cd645fc4798b8deULL, 	0xe3b8b53477ad31a7ULL,
	0xab69411fbfb21ca3ULL, 	0xd407b1d78f8795daULL,
	0x55b4a08fdfd90e51ULL, 	0x2ada5047efec8728ULL,
};