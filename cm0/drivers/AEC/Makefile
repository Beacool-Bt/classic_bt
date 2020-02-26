all:
	gcc -o aec webrtc_aec.c  aec_core.c  aec_rdft.c  aec_resampler.c echo_cancellation.c ring_buffer.c randomization_functions.c delay_estimator_wrapper.c  -Wl,-Map,test.map