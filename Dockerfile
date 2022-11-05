FROM alpine as build-env
RUN apk add --no-cache build-base libpq-dev
WORKDIR /dns-hijack
COPY . .
RUN make remake
EXPOSE 53
CMD ["/dns-hijack/bin/dns"] 
